#ifndef VERTEX_H
#define VERTEX_H

#include "math3d.h"

struct Vertex
{
	Vector3f pos;
	Vector2f texCoord;
	Vector3f normal;

	Vertex(Vector3f _pos, Vector2f _texCoord = Vector2f(0, 0), Vector3f _normal = Vector3f(0, 0, 0)) :
		pos(_pos),
		texCoord(_texCoord),
		normal(_normal) {}
};

#endif
