#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shape.h"


GLfloat cubeVertices[] =
{
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};


GLfloat planeVertices[] =
{ 
    0.5f,  -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    0.5f,  -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    0.5f,  -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f
};


GLfloat quadVertices[] = {
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};


Shape::Shape()
{

}

Shape::~Shape()
{

}


void Shape::setShape(std::string type, glm::vec3 position)
{
    // Set shape properties
    this->shapeType = type;
    this->shapePosition = position;
    this->shapeScale = glm::vec3(1.0f, 1.0f, 1.0f); // Default scale
    this->shapeAngle = 0; // Default rotation angle
    this->shapeRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f); // Default rotation axis

    // Generate and bind VAO and VBO
    glGenVertexArrays(1, &shapeVAO);
    glGenBuffers(1, &shapeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->shapeVBO);

    // Load vertex data based on shape type
    if (type == "cube")
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    else if (type == "plane")
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    else if (type == "quad")
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glBindVertexArray(this->shapeVAO);

    if (type == "quad")
    {
        // For quad: position + texture coordinates
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    else
    {
        // For cube and plane: position + normal + texture coordinates
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    }

    glBindVertexArray(0);
}


void Shape::drawShape(Shader& lightingShader, glm::mat4& view, glm::mat4& projection, Camera& camera)
{
    // Use the shader program
    lightingShader.useShader();

    // Set shader uniforms for view, projection, and model matrices
    GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
    GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
    GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.cameraPosition.x, camera.cameraPosition.y, camera.cameraPosition.z);

    // Compute model matrix and set it in the shader
    glm::mat4 model;
    model = glm::translate(model, this->shapePosition);
    model = glm::scale(model, this->shapeScale);
    model = glm::rotate(model, this->shapeAngle, this->shapeRotationAxis);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Bind VAO and draw the shape based on its type
    glBindVertexArray(this->shapeVAO);

    if (this->shapeType == "cube")
        glDrawArrays(GL_TRIANGLES, 0, 36);
    else if (this->shapeType == "plane")
        glDrawArrays(GL_TRIANGLES, 0, 6);
    else if (this->shapeType == "quad")
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
}


void Shape::drawShape()
{
    // Bind VAO and draw the shape without shader updates
    glBindVertexArray(this->shapeVAO);

    if (this->shapeType == "cube")
        glDrawArrays(GL_TRIANGLES, 0, 36);
    else if (this->shapeType == "plane")
        glDrawArrays(GL_TRIANGLES, 0, 6);
    else if (this->shapeType == "quad")
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
}



std::string Shape::getShapeType()
{
    return this->shapeType;
}


glm::vec3 Shape::getShapePosition()
{
    return this->shapePosition;
}


GLfloat Shape::getShapeAngle()
{
    return this->shapeAngle;
}


glm::vec3 Shape::getShapeRotationAxis()
{
    return this->shapeRotationAxis;
}


glm::vec3 Shape::getShapeScale()
{
    return this->shapeScale;
}


GLuint Shape::getShapeVAO()
{
    return this->shapeVAO;
}


void Shape::setShapePosition(glm::vec3 position)
{
    this->shapePosition = position;
}


void Shape::setShapeAngle(GLfloat angle)
{
    this->shapeAngle = angle;
}


void Shape::setShapeRotationAxis(glm::vec3 rotationAxis)
{
    this->shapeRotationAxis = rotationAxis;
}


void Shape::setShapeScale(glm::vec3 scale)
{
    this->shapeScale = scale;
}

///