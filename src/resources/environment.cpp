#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "environment.h"


Skybox::Skybox()
{

}


Skybox::~Skybox()
{

}

void Skybox::setSkyboxTexture(const char* texPath)
{
    // Set HDR texture for the skybox
    this->texSkybox.setTextureHDR(texPath, "cubemapHDR", true);
}

void Skybox::renderToShader(Shader& shaderSkybox, glm::mat4& projection, glm::mat4& view)
{
    // Activate the shader
    shaderSkybox.useShader();

    // Activate and bind the texture
    glActiveTexture(GL_TEXTURE0);
    this->texSkybox.useTexture();

    // Set shader uniforms
    GLint envMapLoc = glGetUniformLocation(shaderSkybox.Program, "envMap");
    GLint inverseViewLoc = glGetUniformLocation(shaderSkybox.Program, "inverseView");
    GLint inverseProjLoc = glGetUniformLocation(shaderSkybox.Program, "inverseProj");
    GLint apertureLoc = glGetUniformLocation(shaderSkybox.Program, "cameraAperture");
    GLint shutterSpeedLoc = glGetUniformLocation(shaderSkybox.Program, "cameraShutterSpeed");
    GLint isoLoc = glGetUniformLocation(shaderSkybox.Program, "cameraISO");

    // Pass texture unit to the shader
    glUniform1i(envMapLoc, 0);

    // Pass matrices to the shader
    glUniformMatrix4fv(inverseViewLoc, 1, GL_FALSE, glm::value_ptr(glm::transpose(view)));
    glUniformMatrix4fv(inverseProjLoc, 1, GL_FALSE, glm::value_ptr(glm::inverse(projection)));

    // Pass camera settings to the shader
    glUniform1f(apertureLoc, this->cameraAperture);
    glUniform1f(shutterSpeedLoc, this->cameraShutterSpeed);
    glUniform1f(isoLoc, this->cameraISO);
}

void Skybox::setExposure(GLfloat aperture, GLfloat shutterSpeed, GLfloat iso)
{
    // Set exposure-related parameters
    this->cameraAperture = aperture;
    this->cameraShutterSpeed = shutterSpeed;
    this->cameraISO = iso;
}
