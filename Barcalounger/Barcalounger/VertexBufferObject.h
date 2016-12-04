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
	void CreateVBO(int _a_iSize = 0);
	void DeleteVBO();

	void* MapBufferToMemory(int _iUsageHint);
	void* MapSubBufferToMemory(int _iUsageHint, UINT _uiOffset, UINT _uiLength);
	void UnmapBuffer();

	void BindVBO(int _a_iBufferType = GL_ARRAY_BUFFER);
	void UploadDataToGPU(int _iUsageHint);

	void AddData(void* _ptrData, UINT _uiDataSize);

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