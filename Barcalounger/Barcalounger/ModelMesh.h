#ifndef MODEL_MESH_H
#define MODEL_MESH_H

#include "vertex.h"
#include "referenceCounter.h"
#include <string>
#include <map>

class MeshData : public ReferenceCounter
{
public:
	MeshData(int indexSize);
	virtual ~MeshData();

	inline unsigned int GetVBO() { return vbo; }
	inline unsigned int GetIBO() { return ibo; }
	inline int GetSize() { return size; }
protected:
private:
	MeshData(MeshData& _other) {}
	void operator=(MeshData& _other) {}

	unsigned int vbo;
	unsigned int ibo;
	int size;
};

class Mesh
{
public:
	Mesh(const std::string& _fileName);
	Mesh(Vertex* _vertices, int _vertSize, int* _indices, int _indexSize, bool _calcNormals);
	virtual ~Mesh();

	void Draw() const;
protected:
private:
	static std::map<std::string, MeshData*> s_resourceMap;

	Mesh(Mesh& _mesh) {}
	void operator=(Mesh& _mesh) {}

	void CalcNormals(Vertex* _vertices, int _vertSize, int* _indices, int _indexSize);
	void InitMesh(Vertex* _vertices, int _vertSize, int* _indices, int _indexSize, bool _calcNormals = true);

	std::string fileName;
	MeshData* meshData;
};

#endif //!MODEL_MESH_H
