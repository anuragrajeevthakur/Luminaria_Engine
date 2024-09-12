#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "light.h"
#include "shape.h"

// Constructor
Light::Light()
{
    // Initialize default values if needed
}

// Destructor
Light::~Light()
{
    // Clean up resources if needed
}

// Set up a point light with position, color, radius, and whether it's represented as a mesh
void Light::setLight(glm::vec3 position, glm::vec4 color, float radius, bool isMesh)
{
    this->lightType = "point";             // Set light type to point
    this->lightPosition = position;        // Set light position
    this->lightColor = color;              // Set light color
    this->lightRadius = radius;            // Set light radius
    this->lightPointID = lightPointCount;  // Assign ID based on the current count
    this->lightToMesh = isMesh;            // Set whether the light should be represented as a mesh

    if (this->lightToMesh)
    {
        // Set up a mesh for the light if needed
        this->lightMesh.setShape("quad", glm::vec3(0.0f, 0.0f, 0.0f));
        this->lightMesh.setShapePosition(this->lightPosition);
        this->lightMesh.setShapeScale(glm::vec3(0.15f, 0.15f, 0.15f));
    }

    lightPointCount = ++lightPointCount;   // Increment the point light count
    lightPointList.push_back(*this);       // Add this light to the list of point lights
}

// Set up a directional light with direction and color
void Light::setLight(glm::vec3 direction, glm::vec4 color)
{
    this->lightType = "directional";       // Set light type to directional
    this->lightDirection = direction;      // Set light direction
    this->lightColor = color;              // Set light color
    this->lightDirectionalID = lightDirectionalCount; // Assign ID based on the current count

    lightDirectionalCount = ++lightDirectionalCount; // Increment the directional light count
    lightDirectionalList.push_back(*this);           // Add this light to the list of directional lights
}

// Render light information to the shader
void Light::renderToShader(Shader& shader, Camera& camera)
{
    shader.useShader(); // Activate the shader program

    // Cache the uniform locations to avoid multiple lookups
    GLint lightPositionLoc = -1;
    GLint lightColorLoc = -1;
    GLint lightRadiusLoc = -1;
    GLint lightDirectionLoc = -1;

    if (this->lightType == "point")
    {
        // Compute the light's position in view space
        glm::vec3 lightPositionViewSpace = glm::vec3(camera.GetViewMatrix() * glm::vec4(this->lightPosition, 1.0f));

        // Cache uniform locations if not already cached
        if (lightPositionLoc == -1)
            lightPositionLoc = glGetUniformLocation(shader.Program, ("lightPointArray[" + std::to_string(this->lightPointID) + "].position").c_str());
        if (lightColorLoc == -1)
            lightColorLoc = glGetUniformLocation(shader.Program, ("lightPointArray[" + std::to_string(this->lightPointID) + "].color").c_str());
        if (lightRadiusLoc == -1)
            lightRadiusLoc = glGetUniformLocation(shader.Program, ("lightPointArray[" + std::to_string(this->lightPointID) + "].radius").c_str());

        // Set point light uniforms in the shader
        glUniform3f(lightPositionLoc, lightPositionViewSpace.x, lightPositionViewSpace.y, lightPositionViewSpace.z);
        glUniform4f(lightColorLoc, this->lightColor.r, this->lightColor.g, this->lightColor.b, this->lightColor.a);
        glUniform1f(lightRadiusLoc, this->lightRadius);
    }
    else if (this->lightType == "directional")
    {
        // Compute the light's direction in view space
        glm::vec3 lightDirectionViewSpace = glm::vec3(camera.GetViewMatrix() * glm::vec4(this->lightDirection, 0.0f));

        // Cache uniform locations if not already cached
        if (lightDirectionLoc == -1)
            lightDirectionLoc = glGetUniformLocation(shader.Program, ("lightDirectionalArray[" + std::to_string(this->lightDirectionalID) + "].direction").c_str());
        if (lightColorLoc == -1)
            lightColorLoc = glGetUniformLocation(shader.Program, ("lightDirectionalArray[" + std::to_string(this->lightDirectionalID) + "].color").c_str());

        // Set directional light uniforms in the shader
        glUniform3f(lightDirectionLoc, lightDirectionViewSpace.x, lightDirectionViewSpace.y, lightDirectionViewSpace.z);
        glUniform4f(lightColorLoc, this->lightColor.r, this->lightColor.g, this->lightColor.b, this->lightColor.a);
    }
}


// Get the type of light (point or directional)
std::string Light::getLightType()
{
    if (this->lightType == "point")
        return lightPointList[this->lightPointID].lightType;
    if (this->lightType == "directional")
        return lightDirectionalList[this->lightPointID].lightType;
}

// Get the position of a point light
glm::vec3 Light::getLightPosition()
{
    return lightPointList[this->lightPointID].lightPosition;
}

// Get the direction of a directional light
glm::vec3 Light::getLightDirection()
{
    return lightDirectionalList[this->lightDirectionalID].lightDirection;
}

// Get the color of the light
glm::vec4 Light::getLightColor()
{
    if (this->lightType == "point")
        return lightPointList[this->lightPointID].lightColor;
    if (this->lightType == "directional")
        return lightDirectionalList[this->lightPointID].lightColor;
}

// Get the radius of a point light
float Light::getLightRadius()
{
    return lightPointList[this->lightPointID].lightRadius;
}

// Get the ID of the light
GLuint Light::getLightID()
{
    if (this->lightType == "point")
        return lightPointID;
    if (this->lightType == "directional")
        return lightDirectionalID;
}

// Check if the light is represented as a mesh
bool Light::isMesh()
{
    return lightToMesh;
}

// Set the position of a point light
void Light::setLightPosition(glm::vec3 position)
{
    lightPointList[this->lightPointID].lightPosition = position;
    lightPointList[this->lightPointID].lightMesh.setShapePosition(position);
}

// Set the direction of a directional light
void Light::setLightDirection(glm::vec3 direction)
{
    lightDirectionalList[this->lightDirectionalID].lightDirection = direction;
}

// Set the color of the light
void Light::setLightColor(glm::vec4 color)
{
    if (this->lightType == "point")
        lightPointList[this->lightPointID].lightColor = color;
    if (this->lightType == "directional")
        lightDirectionalList[this->lightDirectionalID].lightColor = color;
}

// Set the radius of a point light
void Light::setLightRadius(float radius)
{
    lightPointList[this->lightPointID].lightRadius = radius;
}

// Static member initialization
GLuint Light::lightPointCount = 0;
GLuint Light::lightDirectionalCount = 0;

std::vector<Light> Light::lightPointList;
std::vector<Light> Light::lightDirectionalList;
