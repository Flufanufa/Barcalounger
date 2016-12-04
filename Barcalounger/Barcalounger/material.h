#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
#include "math3d.h"
#include "mappedValues.h"
#include <map>

class Material : public MappedValues
{
public:
	Material() {}

	virtual ~Material()
	{
		//Delete all textures
		std::map<std::string, Texture*>::iterator it;
		for (it = textureMap.begin(); it != textureMap.end(); it++)
			if (it->second) delete it->second;
	}

	Material(Texture* _diffuse, float _specularIntensity, float _specularPower)
	{
		AddTexture("diffuse", _diffuse);
		AddFloat("specularIntensity", _specularIntensity);
		AddFloat("specularPower", _specularPower);
	}

	inline void AddTexture(const std::string& _name, Texture* _value) { textureMap.insert(std::pair<std::string, Texture*>(_name, _value)); }

	inline Texture* GetTexture(const std::string& _name) const
	{
		std::map<std::string, Texture*>::const_iterator it = textureMap.find(_name);
		if (it != textureMap.end())
			return it->second;

		return 0;
	}
protected:
private:
	Material(const Material& _other) {}
	void operator=(const Material& _other) {}

	std::map<std::string, Texture*> textureMap;
};

#endif