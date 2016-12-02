#pragma once
#include <list>
#include <vector>
//#include <iostream>
#include <string>
#include <cimport.h>

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include "LogManager.h"
#include "VertexBufferObject.h"

class ModelManager
{
private:
	std::list<std::string> nameOfResource;

	const struct aiScene* scene = NULL;
	aiVector3D *scene_min, *scene_max, *scene_center;

	static ModelManager *instance;
	ModelManager();
public:
	~ModelManager();
	static ModelManager *getInstance();
	aiScene *getScene();
	int loadasset(const char* path);
	void get_bounding_box(aiVector3D* min, aiVector3D* max);
	void get_bounding_box_for_node(aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo);
	
	VertexBufferObject vboModelData;
	std::vector<int> iMeshStartIndices;
	std::vector<int> iMeshSizes;
	std::vector<int> iMaterialIndices;
};
