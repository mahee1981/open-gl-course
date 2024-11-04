#include "../include/Model.h"

Model::Model()
{
}

Model::~Model()
{
	ClearModel();
}

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (scene == nullptr) {
		std::cout << "Model (" << fileName << ") failed to load " << importer.GetErrorString() << std::endl;
		return;
	}

	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);

}

void Model::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];

		if (materialIndex < textureList.size() && textureList[materialIndex] != nullptr)
		{
			textureList[materialIndex]->UseTextureOnTextureUnit(GL_TEXTURE0);
		}

		meshList[i]->RenderMesh();
	}
}

void Model::ClearModel()
{
	std::cout << "Model deleted" << std::endl;

	//for (size_t i = 0; i < meshList.size(); i++)
	//{
	//	if (meshList[i])
	//	{
	//		delete meshList[i];
	//		meshList[i] = nullptr;
	//	}
	//}

	//for (size_t i = 0; i < textureList.size(); i++)
	//{
	//	if (textureList[i])
	//	{
	//		delete textureList[i];
	//		textureList[i] = nullptr;
	//	}
	//}
}



void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {
		LoadNode(node->mChildren[i], scene);
	}

}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	//formatting for our mesh
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x,  mesh->mVertices[i].y, mesh->mVertices[i].z });

		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x,  mesh->mTextureCoords[0][i].y });
		}
		else 
		{
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x,  -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	meshList.push_back(std::move(std::make_unique<Mesh>(vertices, indices)));
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material(scene->mMaterials[i]);
		
		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				std::string texPath = std::string("Textures/") + filename;

				textureList[i] = std::move(std::make_unique<Texture>(texPath.c_str()));

				if (!textureList[i]->LoadTextureWithAlpha())
				{
					std::cout << "Failed to load texture at: " << texPath << std::endl;
				}
			}
		}

		if (textureList[i] == nullptr)
		{
			textureList[i] = std::move(std::make_unique<Texture>("Textures/plain.png"));
			textureList[i]->LoadTextureWithAlpha();

		}

	}
}
