#include "texture.h"
#include "stb_image.h"
#include <iostream>
#include <cassert>

std::map<std::string, TextureData*> Texture::s_resourceMap;

TextureData::TextureData(GLenum _textureTarget)
{
	glGenTextures(1, &textureID);
	textureTarget = _textureTarget;
}

TextureData::~TextureData()
{
	if (textureID) glDeleteTextures(1, &textureID);
}


Texture::Texture(const std::string& _fileName, GLenum _textureTarget, GLfloat _filter)
{
	fileName = _fileName;

	std::map<std::string, TextureData*>::const_iterator it = s_resourceMap.find(_fileName);
	if (it != s_resourceMap.end())
	{
		textureData = it->second;
		textureData->AddReference();
	}
	else
	{
		int x, y, bytesPerPixel;
		unsigned char* data = stbi_load(("../resources/textures/" + _fileName).c_str(), &x, &y, &bytesPerPixel, 4);

		if (data == NULL)
		{
			std::cerr << "Unable to load texture: " << _fileName << std::endl;
		}

		InitTexture(x, y, data, _textureTarget, _filter);
		stbi_image_free(data);

		s_resourceMap.insert(std::pair<std::string, TextureData*>(_fileName, textureData));
	}
}

Texture::Texture(int _width, int _height, unsigned char* _data, GLenum _textureTarget, GLfloat _filter)
{
	fileName = "";
	InitTexture(_width, _height, _data, _textureTarget, _filter);
}

Texture::~Texture()
{
	if (textureData && textureData->RemoveReference())
	{
		if (fileName.length() > 0)
			s_resourceMap.erase(fileName);

		delete textureData;
	}
}

void Texture::InitTexture(int _width, int _height, unsigned char* _data, GLenum _textureTarget, GLfloat _filter)
{
	if (_width > 0 && _height > 0 && _data != 0)
	{
		textureData = new TextureData(_textureTarget);
		glBindTexture(_textureTarget, textureData->GetTextureID());

		glTexParameterf(_textureTarget, GL_TEXTURE_MIN_FILTER, _filter);
		glTexParameterf(_textureTarget, GL_TEXTURE_MAG_FILTER, _filter);

		glTexImage2D(_textureTarget, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
	}
}

void Texture::Bind(unsigned int _unit) const
{
	assert(_unit >= 0 && _unit <= 31);
	glActiveTexture(GL_TEXTURE0 + _unit);
	glBindTexture(textureData->GetTextureTarget(), textureData->GetTextureID());
}