#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include "stb_image.h"
#include "texture.h"


Texture::Texture()
{

}


Texture::~Texture()
{
    glDeleteTextures(1, &this->texID);
}


void Texture::setTexture(const char* texPath, std::string texName, bool texFlip)
{
    // Set texture type to 2D
    this->texType = GL_TEXTURE_2D;

    // Convert texture path to a string
    std::string tempPath = std::string(texPath);

    // Flip texture vertically if required
    if (texFlip)
        stbi_set_flip_vertically_on_load(true);
    else
        stbi_set_flip_vertically_on_load(false);

    // Generate and bind texture
    glGenTextures(1, &this->texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texID);

    // Set anisotropic filtering for better texture quality
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoFilterLevel);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, this->anisoFilterLevel);

    // Load texture data
    int width, height, numComponents;
    unsigned char* texData = stbi_load(tempPath.c_str(), &width, &height, &numComponents, 0);

    // Store texture properties
    this->texWidth = width;
    this->texHeight = height;
    this->texComponents = numComponents;
    this->texName = texName;

    // If texture data is loaded correctly
    if (texData)
    {
        // Determine texture format based on the number of components
        if (numComponents == 1)
            this->texFormat = GL_RED;
        else if (numComponents == 3)
            this->texFormat = GL_RGB;
        else if (numComponents == 4)
            this->texFormat = GL_RGBA;

        this->texInternalFormat = this->texFormat;

        // Create texture
        glTexImage2D(GL_TEXTURE_2D, 0, this->texInternalFormat, this->texWidth, this->texHeight, 0, this->texFormat, GL_UNSIGNED_BYTE, texData);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Enable mipmaps and linear filtering
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        // Log error if texture loading fails
        std::cerr << "TEXTURE FAILED - LOADING : " << texPath << std::endl;
    }

    // Free texture data and unbind texture
    stbi_image_free(texData);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::setTextureHDR(const char* texPath, std::string texName, bool texFlip)
{
    // Set texture type to 2D
    this->texType = GL_TEXTURE_2D;

    // Convert texture path to a string
    std::string tempPath = std::string(texPath);

    // Flip texture vertically if required
    if (texFlip)
        stbi_set_flip_vertically_on_load(true);
    else
        stbi_set_flip_vertically_on_load(false);

    // Generate and bind texture
    glGenTextures(1, &this->texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texID);

    // Check if the file is an HDR image
    if (stbi_is_hdr(tempPath.c_str()))
    {
        int width, height, numComponents;
        float* texData = stbi_loadf(tempPath.c_str(), &width, &height, &numComponents, 0);

        // Store texture properties
        this->texWidth = width;
        this->texHeight = height;
        this->texComponents = numComponents;
        this->texName = texName;

        // If texture data is loaded correctly
        if (texData)
        {
            // Set internal and external formats based on the number of components
            if (numComponents == 3)
            {
                this->texInternalFormat = GL_RGB32F;
                this->texFormat = GL_RGB;
            }
            else if (numComponents == 4)
            {
                this->texInternalFormat = GL_RGBA32F;
                this->texFormat = GL_RGBA;
            }

            // Create HDR texture
            glTexImage2D(GL_TEXTURE_2D, 0, this->texInternalFormat, this->texWidth, this->texHeight, 0, this->texFormat, GL_FLOAT, texData);

            // Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Generate mipmaps
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            // Log error if HDR texture loading fails
            std::cerr << "HDR TEXTURE - FAILED LOADING : " << texPath << std::endl;
        }

        // Free texture data
        stbi_image_free(texData);
    }
    else
    {
        // Log error if the file is not HDR
        std::cerr << "HDR TEXTURE - FILE IS NOT HDR : " << texPath << std::endl;
    }

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::setTextureHDR(GLuint width, GLuint height, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter)
{
    this->texType = GL_TEXTURE_2D;

    // Generate and bind the texture
    glGenTextures(1, &this->texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texID);

    // Set texture width, height, and format
    this->texWidth = width;
    this->texHeight = height;
    this->texFormat = format;
    this->texInternalFormat = internalFormat;

    // Determine number of components based on format
    if (format == GL_RED)
        this->texComponents = 1;
    else if (format == GL_RG)
        this->texComponents = 2;
    else if (format == GL_RGB)
        this->texComponents = 3;
    else if (format == GL_RGBA)
        this->texComponents = 4;

    // Allocate texture memory without initializing it (HDR texture)
    glTexImage2D(GL_TEXTURE_2D, 0, this->texInternalFormat, this->texWidth, this->texHeight, 0, this->texFormat, GL_FLOAT, nullptr);

    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate mipmaps for the texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::setTextureCube(std::vector<const char*>& faces, bool texFlip)
{
    this->texType = GL_TEXTURE_CUBE_MAP;

    std::vector<std::string> cubemapFaces;

    // Convert the input vector of face file paths to strings
    for (GLuint j = 0; j < faces.size(); j++)
    {
        std::string tempPath = std::string(faces[j]);
        cubemapFaces.push_back(tempPath);
    }

    // Set vertical flip based on the texFlip flag
    if (texFlip)
        stbi_set_flip_vertically_on_load(true);
    else
        stbi_set_flip_vertically_on_load(false);

    // Generate and bind the cube map texture
    glGenTextures(1, &this->texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(this->texType, this->texID);

    int width, height, numComponents;
    unsigned char* texData;

    // Load each face of the cubemap
    for (GLuint i = 0; i < 6; i++)
    {
        texData = stbi_load(cubemapFaces[i].c_str(), &width, &height, &numComponents, 0);

        // If texture dimensions and components are not set, initialize them
        if (this->texWidth == NULL && this->texHeight == NULL && this->texComponents == NULL)
        {
            this->texWidth = width;
            this->texHeight = height;
            this->texComponents = numComponents;
        }

        if (texData)
        {
            // Determine format based on number of components
            if (numComponents == 1)
                this->texFormat = GL_RED;
            else if (numComponents == 3)
                this->texFormat = GL_RGB;
            else if (numComponents == 4)
                this->texFormat = GL_RGBA;
            this->texInternalFormat = this->texFormat;

            // Upload the texture data for the current face
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, this->texInternalFormat, this->texWidth, this->texHeight, 0, this->texFormat, GL_UNSIGNED_BYTE, texData);

            // Set texture parameters for wrapping and filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Generate mipmaps for the current face
            glGenerateMipmap(this->texType);
        }
        else
        {
            // Handle texture loading failure
            std::cerr << "CUBEMAP TEXTURE - FAILED LOADING : " << cubemapFaces[i] << std::endl;
        }

        // Free the loaded image data
        stbi_image_free(texData);
    }

    // Set cube map specific wrapping options
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Unbind the texture
    glBindTexture(this->texType, 0);
}


void Texture::setTextureCube(GLuint width, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter)
{
    this->texType = GL_TEXTURE_CUBE_MAP;

    // Generate and bind the cube map texture
    glGenTextures(1, &this->texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(this->texType, this->texID);

    // Allocate memory for all six faces of the cube map
    for (GLuint i = 0; i < 6; ++i)
    {
        // If dimensions and components are not set, initialize them
        if (this->texWidth == NULL && this->texHeight == NULL && this->texComponents == NULL)
        {
            this->texWidth = width;
            this->texHeight = width;
            this->texFormat = format;
            this->texInternalFormat = internalFormat;
        }

        // Determine number of components based on format
        if (format == GL_RED)
            this->texComponents = 1;
        else if (format == GL_RGB)
            this->texComponents = 3;
        else if (format == GL_RGBA)
            this->texComponents = 4;

        // Allocate memory for the current face of the cube map
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, this->texInternalFormat, this->texWidth, this->texHeight, 0, this->texFormat, type, nullptr);
    }

    // Set texture parameters for filtering and wrapping
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Unbind the texture
    glBindTexture(this->texType, 0);
}



void Texture::computeTexMipmap()
{
    glBindTexture(this->texType, this->texID);
    glGenerateMipmap(this->texType);
}


GLuint Texture::getTexID()
{
    return this->texID;
}


GLuint Texture::getTexWidth()
{
    return this->texWidth;
}


GLuint Texture::getTexHeight()
{
    return this->texHeight;
}


std::string Texture::getTexName()
{
    return this->texName;
}


void Texture::useTexture()
{
    glBindTexture(this->texType, this->texID);
}
