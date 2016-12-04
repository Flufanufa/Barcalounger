#ifndef TEXTURE_H
#define TEXTURE_H

#include "referenceCounter.h"
#include <GL/glew.h>
#include <string>
#include <map>

class TextureData : public ReferenceCounter
{
public:
	TextureData(GLenum _textureTarget);
	virtual ~TextureData();

	inline GLenum GetTextureTarget() { return textureTarget; }
	inline GLuint GetTextureID() { return textureID; }
protected:
private:
	TextureData(TextureData& _other) {}
	void operator=(TextureData& _other) {}

	GLenum textureTarget;
	GLuint textureID;
};

class Texture
{
public:
	Texture(const std::string& _fileName, GLenum _textureTarget = GL_TEXTURE_2D, GLfloat _filter = GL_LINEAR);
	Texture(int _width = 0, int _height = 0, unsigned char* _data = 0, GLenum _textureTarget = GL_TEXTURE_2D, GLfloat _filter = GL_LINEAR);
	virtual ~Texture();

	void Bind(unsigned int _unit = 0) const;
protected:
private:
	static std::map<std::string, TextureData*> s_resourceMap;

	Texture(Texture& _texture) {}
	void operator=(Texture& _texture) {}

	TextureData* textureData;
	std::string fileName;

	void InitTexture(int _width, int _height, unsigned char* _data, GLenum _textureTarget, GLfloat _filter);
};

#endif