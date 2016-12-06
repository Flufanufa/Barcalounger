#include "ModelMesh.h"
#include <GL/glew.h>
#include <iostream>

#include <vector>
#include <cassert>

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

std::map<std::string, MeshData*> Mesh::s_resourceMap;

MeshData::MeshData(int _indexSize) : ReferenceCounter()
{
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	size = _indexSize;
}

MeshData::~MeshData()
{
	if (vbo) glDeleteBuffers(1, &vbo);
	if (ibo) glDeleteBuffers(1, &ibo);
}


Mesh::Mesh(Vertex* _vertices, int _vertSize, int* _indices, int _indexSize, bool _calcNormals)
{
	fileName = "";
	InitMesh(_vertices, _vertSize, _indices, _indexSize, _calcNormals);
}

Mesh::Mesh(const std::string& _fileName)
{
	fileName = _fileName;
	meshData = 0;

	std::map<std::string, MeshData*>::const_iterator it = s_resourceMap.find(_fileName);
	if (it != s_resourceMap.end())
	{
		meshData = it->second;
		meshData->AddReference();
	}
	else
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(_fileName.c_str(),
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs);

		if (!scene)
		{
			//std::cout << "Mesh load failed!: " << _fileName << std::endl;
			assert(0 == 0);
		}

		const aiMesh* model = scene->mMeshes[0];

		std::vector<Vertex> vertices;
		std::vector<int> indices;

		const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
		for (unsigned int i = 0; i < model->mNumVertices; i++)
		{
			const aiVector3D* pPos = &(model->mVertices[i]);
			const aiVector3D* pNormal = &(model->mNormals[i]);
			const aiVector3D* pTexCoord = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][i]) : &aiZeroVector;

			Vertex vert(Vector3f(pPos->x, pPos->y, pPos->z),
				Vector2f(pTexCoord->x, pTexCoord->y),
				Vector3f(pNormal->x, pNormal->y, pNormal->z));

			vertices.push_back(vert);
		}

		for (unsigned int i = 0; i < model->mNumFaces; i++)
		{
			const aiFace& face = model->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		InitMesh(&vertices[0], vertices.size(), (int*)&indices[0], indices.size(), false);

		s_resourceMap.insert(std::pair<std::string, MeshData*>(_fileName, meshData));
	}
}

Mesh::~Mesh()
{
	if (meshData && meshData->RemoveReference())
	{
		if (fileName.length() > 0)
			s_resourceMap.erase(fileName);

		delete meshData;
	}
}

void Mesh::InitMesh(Vertex* _vertices, int _vertSize, int* _indices, int _indexSize, bool _calcNormals)
{
	meshData = new MeshData(_indexSize);

	if (_calcNormals)
		this->CalcNormals(_vertices, _vertSize, _indices, _indexSize);

	glBindBuffer(GL_ARRAY_BUFFER, meshData->GetVBO());
	glBufferData(GL_ARRAY_BUFFER, _vertSize * sizeof(Vertex), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->GetIBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexSize * sizeof(int), _indices, GL_STATIC_DRAW);
}

void Mesh::Draw() const
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, meshData->GetVBO());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3f));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f) + sizeof(Vector2f)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->GetIBO());
	glDrawElements(GL_TRIANGLES, meshData->GetSize(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Mesh::CalcNormals(Vertex* _vertices, int _vertSize, int* _indices, int _indexSize)
{
	for (int i = 0; i < _indexSize; i += 3)
	{
		int i0 = _indices[i];
		int i1 = _indices[i + 1];
		int i2 = _indices[i + 2];

		Vector3f v1 = _vertices[i1].pos - _vertices[i0].pos;
		Vector3f v2 = _vertices[i2].pos - _vertices[i0].pos;

		Vector3f normal = v1.Cross(v2).Normalized();

		_vertices[i0].normal += normal;
		_vertices[i1].normal += normal;
		_vertices[i2].normal += normal;
	}

	for (int i = 0; i < _vertSize; i++)
		_vertices[i].normal = _vertices[i].normal.Normalized();
}
