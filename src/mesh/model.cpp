#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "model.h"
#include "mesh.h"

Model::Model()
{
}

Model::~Model()
{
}

// Function to load a model from a file using Assimp
void Model::loadModel(std::string path)
{
    // Import model file with specific processing options
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    // Error checking if the model fails to load
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    // Set directory path for textures or other related assets
    this->directory = path.substr(0, path.find_last_of('/'));

    // Process the root node recursively to build meshes
    this->processNode(scene->mRootNode, scene);
}

// Function to draw all meshes in the model
void Model::Draw()
{
    for (GLuint i = 0; i < this->meshes.size(); i++)
        this->meshes[i].Draw();  // Render each mesh
}

// Recursive function to process nodes in the model's scene graph
void Model::processNode(aiNode* node, const aiScene* scene)
{
    // Process each mesh in the current node
    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(this->processMesh(mesh, scene));
    }

    // Recursively process each child node
    for (GLuint i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene);
    }
}

// Function to process and convert Assimp's aiMesh into a Mesh object
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;  // List of vertices for the mesh
    std::vector<GLuint> indices;   // List of indices for the mesh

    // Process vertices
    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;

        // Position
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // Normal
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        // Texture Coordinates (if available)
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);  // Default texture coordinates if none are provided
        }

        vertices.push_back(vertex);  // Add vertex to the list
    }

    // Process faces (triangles) and store their indices
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        // Each face has multiple indices
        for (GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Mesh(vertices, indices);
}
