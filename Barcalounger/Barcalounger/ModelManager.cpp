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

int ModelManager::loadasset(const char* _path)
{
	Assimp::Importer importer;
	scene = importer.ReadFile(_path, aiProcess_Triangulate); //ensures that all points are in sets of 3
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

void ModelManager::get_bounding_box(aiVector3D* _min, aiVector3D* _max)
{
	aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	_min->x = _min->y = _min->z = 50.0f;
	_max->x = _max->y = _max->z = -50.0f;
	get_bounding_box_for_node(scene->mRootNode, _min, _max, &trafo);
}

void ModelManager::get_bounding_box_for_node(aiNode* _nd, aiVector3D* _min, aiVector3D* _max, aiMatrix4x4* _trafo) {
	
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *_trafo;
	aiMultiplyMatrix4(_trafo, &_nd->mTransformation);

	for (; n < _nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[_nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp, _trafo);

			_min->x = aisgl_min(_min->x, tmp.x);
			_min->y = aisgl_min(_min->y, tmp.y);
			_min->z = aisgl_min(_min->z, tmp.z);

			_max->x = aisgl_max(_max->x, tmp.x);
			_max->y = aisgl_max(_max->y, tmp.y);
			_max->z = aisgl_max(_max->z, tmp.z);
		}
	}

	for (n = 0; n < _nd->mNumChildren; ++n) {
		get_bounding_box_for_node(_nd->mChildren[n], _min, _max, _trafo);
	}
	*_trafo = prev;
}


