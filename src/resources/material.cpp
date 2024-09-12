#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <tuple>

#include <glad/glad.h>
#include <texture.h>
#include <shader.h>

#include "material.h"

Material::Material()
{

}

Material::~Material()
{

}

void Material::addTexture(std::string uniformName, Texture texObj)
{
    this->texList.push_back(std::tuple<std::string, Texture>(uniformName, texObj));
}

void Material::setShader(Shader& shader)
{
    this->matShader = shader;
}

void Material::renderToShader()
{
    // Activate the shader program
    this->matShader.useShader();

    // Log the size of the texture list
    std::cout << "texList Size : " << this->texList.size() << std::endl;

    // Iterate through the list of textures
    for (GLuint i = 0; i < this->texList.size(); ++i)
    {

        std::string currentUniformName = std::get<0>(this->texList[i]);
        Texture currentTex = std::get<1>(this->texList[i]);

        // Log texture details
        std::cout << "i : " << i << std::endl;
        std::cout << "texWidth : " << currentTex.getTexWidth() << std::endl;
        std::cout << "texHeight : " << currentTex.getTexHeight() << std::endl;
        std::cout << "texUniformName : " << currentTex.getTexName() << std::endl;
        std::cout << "ActiveTexture sent : " << GL_TEXTURE0 + i << std::endl;

        // Activate the texture unit and set the texture
        glActiveTexture(GL_TEXTURE0 + i);
        currentTex.useTexture();

        // Set the texture uniform
        GLint uniformLocation = glGetUniformLocation(this->matShader.Program, currentUniformName.c_str());
        glUniform1i(uniformLocation, i);

        // Log separator for clarity
        std::cout << "------" << std::endl;
    }

    // Log end of texture processing
    std::cout << "============" << std::endl;
}

