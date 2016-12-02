#include "ModelManager.h"
#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

ModelManager *ModelManager::instance = NULL;
ModelManager::ModelManager()
{
}


ModelManager::~ModelManager()
{
}

ModelManager *ModelManager::getInstance() {
	if (instance == NULL) {
		instance = new ModelManager();
	}
	return instance;
}

int ModelManager::loadasset(const char* path)
{
	Assimp::Importer importer;
	scene = importer.ReadFile(path, aiProcess_Triangulate); //ensures that all points are in sets of 3
	if (!scene) { 
		LogManager *logInstance = LogManager::getInstance();
		logInstance->error("model failed to load");
		return 1; 
	}

	const int iVertexTotalSize = sizeof(aiVector3D) * 2 + sizeof(aiVector2D);

	int iTotalVertices = 0;

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		int iMeshFaces = mesh->mNumFaces;
		iMaterialIndices.push_back(mesh->mMaterialIndex);
		int iSizeBefore = vboModelData.GetCurrentSize();
		iMeshStartIndices.push_back(iSizeBefore / iVertexTotalSize);
		
		for (int j = 0; j < iMeshFaces;j++)
		{
			const aiFace& face = mesh->mFaces[j];
			
			for (int k = 0; k < 3;k++)
			{
				aiVector3D pos = mesh->mVertices[face.mIndices[k]];
				aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
				aiVector3D normal = mesh->mNormals[face.mIndices[k]];
				vboModelData.AddData(&pos, sizeof(aiVector3D));
				vboModelData.AddData(&uv, sizeof(aiVector2D));
				vboModelData.AddData(&normal, sizeof(aiVector3D));
			}
		}
		int iMeshVertices = mesh->mNumVertices;
		iTotalVertices += iMeshVertices;
		iMeshSizes.push_back((vboModelData.GetCurrentSize() - iSizeBefore) / iVertexTotalSize);
	}

	/*
	if (scene) {
		get_bounding_box(scene_min, scene_max);
		scene_center->x = (scene_min->x + scene_max->x) / 2.0f;
		scene_center->y = (scene_min->y + scene_max->y) / 2.0f;
		scene_center->z = (scene_min->z + scene_max->z) / 2.0f;
		return 0;
	}*/
	return 0;
}

void ModelManager::get_bounding_box(aiVector3D* min, aiVector3D* max)
{
	aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z = 50.0f;
	max->x = max->y = max->z = -50.0f;
	get_bounding_box_for_node(scene->mRootNode, min, max, &trafo);
}

void ModelManager::get_bounding_box_for_node(aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo) {
	
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo, &nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp, trafo);

			min->x = aisgl_min(min->x, tmp.x);
			min->y = aisgl_min(min->y, tmp.y);
			min->z = aisgl_min(min->z, tmp.z);

			max->x = aisgl_max(max->x, tmp.x);
			max->y = aisgl_max(max->y, tmp.y);
			max->z = aisgl_max(max->z, tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(nd->mChildren[n], min, max, trafo);
	}
	*trafo = prev;
}


