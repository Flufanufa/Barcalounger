#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H

#include "camera.h"
#include "lighting.h"
#include "mappedValues.h"
#include "material.h"
#include <vector>
#include <map>
class GameObject;

class RenderingEngine : public MappedValues
{
public:
	RenderingEngine();

	void Render(GameObject* object);

	inline Camera& GetMainCamera() { return *mainCamera; }
	inline BaseLight* GetActiveLight() { return activeLight; }

	inline void AddLight(BaseLight* _light) { lights.push_back(_light); }
	inline void AddCamera(Camera* _camera) { mainCamera = _camera; }

	inline unsigned int GetSamplerSlot(const std::string& _samplerName) { return samplerMap[_samplerName]; }

	virtual void UpdateUniformStruct(const Transform& _transform, const Material& _material, Shader* _shader,
		const std::string& _uniformName, const std::string& _uniformType)
	{
		throw _uniformType + " is not supported by the rendering engine";
	}

	virtual ~RenderingEngine();
protected:
private:
	RenderingEngine(const RenderingEngine& _other) {}
	void operator=(const RenderingEngine& _other) {}

	Camera* mainCamera;
	BaseLight* activeLight;
	Shader* defaultShader;
	std::vector<BaseLight*> lights;
	std::map<std::string, unsigned int> samplerMap;
};

#endif // RENDERINGENGINE_H
