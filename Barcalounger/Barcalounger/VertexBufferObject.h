#pragma once
#include <vgl.h>
#include <glm\glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
/********************************

Class:		CVertexBufferObject

Purpose:	Wraps OpenGL vertex buffer
object.

********************************/

class VertexBufferObject
{
public:
	void CreateVBO(int a_iSize = 0);
	void DeleteVBO();

	void* MapBufferToMemory(int iUsageHint);
	void* MapSubBufferToMemory(int iUsageHint, UINT uiOffset, UINT uiLength);
	void UnmapBuffer();

	void BindVBO(int a_iBufferType = GL_ARRAY_BUFFER);
	void UploadDataToGPU(int iUsageHint);

	void AddData(void* ptrData, UINT uiDataSize);

	void* GetDataPointer();
	UINT GetBufferID();

	int GetCurrentSize();

	VertexBufferObject();

private:
	UINT uiBuffer;
	int iSize;
	int iCurrentSize;
	int iBufferType;
	std::vector<BYTE> data;

	bool bDataUploaded;
};