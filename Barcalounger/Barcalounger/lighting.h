#ifndef LIGHTING_H
#define LIGHTING_H

#include "math3d.h"
#include "gameComponent.h"

class Engine;

class BaseLight : public GameComponent
{
public:
	Vector3f color;
	float intensity;

	BaseLight(const Vector3f& _color = Vector3f(0, 0, 0), float _intensity = 0) :
		color(_color),
		intensity(_intensity),
		shader(0) {}

	virtual ~BaseLight();

	virtual void AddToEngine(CoreEngine* engine);
	inline Shader* GetShader() { return shader; }

	void SetShader(Shader* _shader);
private:
	BaseLight(BaseLight& _other) {}
	void operator=(BaseLight& _other) {}

	Shader* shader;
};

struct DirectionalLight : public BaseLight
{
	DirectionalLight(const Vector3f& _color = Vector3f(0, 0, 0), float _intensity = 0);
};

struct Attenuation
{
	float constant;
	float linear;
	float exponent;

	Attenuation(float _constant = 0, float _linear = 0, float _exponent = 1) :
		constant(_constant),
		linear(_linear),
		exponent(_exponent) {}
};

struct PointLight : public BaseLight
{
	Attenuation atten;
	float range;

	PointLight(const Vector3f& _color = Vector3f(0, 0, 0), float _intensity = 0, const Attenuation& _atten = Attenuation());
};

struct SpotLight : public PointLight
{
	float cutoff;

	SpotLight(const Vector3f& _color = Vector3f(0, 0, 0), float _intensity = 0, const Attenuation& _atten = Attenuation(), float cutoff = 0);
};

#endif