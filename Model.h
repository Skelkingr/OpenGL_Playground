#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Mesh.h"
#include "Texture.h"
#include "Util.h"

class Model
{
public:
	Model();
	~Model();

	void LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();

private:
	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);

private:
	std::vector<Mesh*> mMeshList;
	std::vector<Texture*> mTextureList;
	std::vector<unsigned int> mMeshToTex;
};

#endif

