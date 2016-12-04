#include "lighting.h"
#include "renderingEngine.h"
#include "shader.h"
#include "Engine.h"

#define COLOR_DEPTH 256

void BaseLight::AddToEngine(CoreEngine* _engine)
{
	_engine->GetRenderingEngine()->AddLight(this);
}

BaseLight::~BaseLight()
{
	if (shader) delete shader;
}

void BaseLight::SetShader(Shader* _shader)
{
	if (shader) delete shader;
	shader = _shader;
}

DirectionalLight::DirectionalLight(const Vector3f& _color, float _intensity) :
	BaseLight(_color, _intensity)
{
	SetShader(new Shader("forward-directional"));
}

PointLight::PointLight(const Vector3f& _color, float _intensity, const Attenuation& _atten) :
	BaseLight(_color, _intensity),
	atten(_atten)
{
	float a = _atten.exponent;
	float b = _atten.linear;
	float c = _atten.constant - COLOR_DEPTH * intensity * _color.Max();

	range = (-b + sqrtf(b*b - 4 * a*c)) / (2 * a);

	SetShader(new Shader("forward-point"));
}

SpotLight::SpotLight(const Vector3f& _color, float _intensity, const Attenuation& _atten, float _cutoff) :
	PointLight(_color, _intensity, _atten),
	cutoff(_cutoff)
{
	SetShader(new Shader("forward-spot"));
}
