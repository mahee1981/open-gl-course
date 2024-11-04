#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../include/Mesh.h"
#include "../include/Texture.h"

class Model
{
public:
	Model();
	Model(const Model&) = delete; // because the class contains unique smart pointers
	Model& operator=(const Model&) = delete;
	~Model();


	void LoadModel(const std::string &fileName);
	void RenderModel();
	void ClearModel();


private:

	void LoadNode(aiNode* node, const aiScene*);
	void LoadMesh(aiMesh* mesh, const aiScene*);
	void LoadMaterials(const aiScene*);

	std::vector<std::unique_ptr<Mesh>> meshList;
	std::vector<std::unique_ptr<Texture>> textureList;
	std::vector<unsigned int> meshToTex;

};

