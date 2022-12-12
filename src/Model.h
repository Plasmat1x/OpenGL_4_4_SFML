#pragma once

#include<gl/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<stb_image.h>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<map>
#include<vector>

#include "Mesh.h"
#include "Shader.h"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

using namespace std;

class Model
{
public:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    Model(std::string const& path, bool gamma = false);

    void Draw(Shader& shader);

private:

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
