#include "VertexBufferObject.h"




VertexBufferObject::VertexBufferObject()
{
	bDataUploaded = false;
	uiBuffer = 0;
}

/*-----------------------------------------------

Name:	CreateVBO

Params:	a_iSize - initial size of buffer

Result:	Creates vertex buffer object.

/*---------------------------------------------*/

void VertexBufferObject::CreateVBO(int _a_iSize)
{
	glGenBuffers(1, &uiBuffer);
	data.reserve(_a_iSize);
	iSize = _a_iSize;
	iCurrentSize = 0;
}

/*-----------------------------------------------

Name:	DeleteVBO

Params:	none

Result:	Releases VBO and frees all memory.

/*---------------------------------------------*/

void VertexBufferObject::DeleteVBO()
{
	glDeleteBuffers(1, &uiBuffer);
	bDataUploaded = false;
	data.clear();
}

/*-----------------------------------------------

Name:	mapBufferToMemory

Params:	iUsageHint - GL_READ_ONLY, GL_WRITE_ONLY...

Result:	Maps whole buffer data to memory and
returns pointer to data.

/*---------------------------------------------*/

void* VertexBufferObject::MapBufferToMemory(int _iUsageHint)
{
	if (!bDataUploaded)return NULL;
	void* ptrRes = glMapBuffer(iBufferType, _iUsageHint);
	return ptrRes;
}

/*-----------------------------------------------

Name:	MapSubBufferToMemory

Params:	iUsageHint - GL_READ_ONLY, GL_WRITE_ONLY...
uiOffset - data offset (from where should
data be mapped).
uiLength - length of data

Result:	Maps specified part of buffer to memory.

/*---------------------------------------------*/

void* VertexBufferObject::MapSubBufferToMemory(int _iUsageHint, UINT _uiOffset, UINT _uiLength)
{
	if (!bDataUploaded)return NULL;
	void* ptrRes = glMapBufferRange(iBufferType, _uiOffset, _uiLength, _iUsageHint);
	return ptrRes;
}

/*-----------------------------------------------

Name:	UnmapBuffer

Params:	none

Result:	Unmaps previously mapped buffer.

/*---------------------------------------------*/

void VertexBufferObject::UnmapBuffer()
{
	glUnmapBuffer(iBufferType);
}

/*-----------------------------------------------

Name:	BindVBO

Params:	a_iBufferType - buffer type (GL_ARRAY_BUFFER, ...)

Result:	Binds this VBO.

/*---------------------------------------------*/

void VertexBufferObject::BindVBO(int _a_iBufferType)
{
	iBufferType = _a_iBufferType;
	glBindBuffer(iBufferType, uiBuffer);
}

/*-----------------------------------------------

Name:	UploadDataToGPU

Params:	iUsageHint - GL_STATIC_DRAW, GL_DYNAMIC_DRAW...

Result:	Sends data to GPU.

/*---------------------------------------------*/

void VertexBufferObject::UploadDataToGPU(int _iDrawingHint)
{
	glBufferData(iBufferType, data.size(), &data[0], _iDrawingHint);
	bDataUploaded = true;
	data.clear();
}

/*-----------------------------------------------

Name:	AddData

Params:	ptrData - pointer to arbitrary data
uiDataSize - data size in bytes

Result:	Adds arbitrary data to VBO.

/*---------------------------------------------*/

void VertexBufferObject::AddData(void* _ptrData, UINT _uiDataSize)
{
	data.insert(data.end(), (BYTE*)_ptrData, (BYTE*)_ptrData + _uiDataSize);
	iCurrentSize += _uiDataSize;
}

/*-----------------------------------------------

Name:	GetDataPointer

Params:	none

Result:	Returns data pointer (only before uploading).

/*---------------------------------------------*/

void* VertexBufferObject::GetDataPointer()
{
	if (bDataUploaded)return NULL;
	return (void*)data[0];
}

/*-----------------------------------------------

Name:	GetBufferID

Params:	none

Result:	Returns VBO ID.

/*---------------------------------------------*/

UINT VertexBufferObject::GetBufferID()
{
	return uiBuffer;
}

/*-----------------------------------------------

Name:	GetCurrentSize

Params:	none

Result: Returns size of data that has been added to
the buffer object.

/*---------------------------------------------*/

int VertexBufferObject::GetCurrentSize()
{
	return iCurrentSize;
}