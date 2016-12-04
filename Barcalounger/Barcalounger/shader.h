#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <vector>
#include <string>

#include "ReferenceCounter.h"
#include "Math3d.h"
#include "material.h"
#include "renderingEngine.h"
#include "lighting.h"
#include "transform.h"

struct TypedData
{
	std::string name;
	std::string type;
};

struct UniformStruct
{
	std::string name;
	std::vector<TypedData> memberNames;
};

class ShaderData : public ReferenceCounter
{
public:
	ShaderData(const std::string& _fileName);
	virtual ~ShaderData();

	inline int GetProgram() { return program; }
	inline std::vector<int>& GetShaders() { return shaders; }
	inline std::vector<std::string>& GetUniformNames() { return uniformNames; }
	inline std::vector<std::string>& GetUniformTypes() { return uniformTypes; }
	inline std::map<std::string, unsigned int>& GetUniformMap() { return uniformMap; }
private:
	void AddVertexShader(const std::string& _text);
	void AddGeometryShader(const std::string& _text);
	void AddFragmentShader(const std::string& _text);
	void AddProgram(const std::string& _text, int _type);

	void AddAllAttributes(const std::string& _vertexShaderText);
	void AddShaderUniforms(const std::string& _shaderText);
	void AddUniform(const std::string& _uniformName, const std::string& _uniformType, const std::vector<UniformStruct>& _structs);
	void CompileShader();

	int program;
	std::vector<int> shaders;
	std::vector<std::string> uniformNames;
	std::vector<std::string> uniformTypes;
	std::map<std::string, unsigned int> uniformMap;
};

class Shader
{
public:
	Shader(const std::string& fileName);
	virtual ~Shader();

	void Bind();
	virtual void UpdateUniforms(const Transform& _transform, const Material& _material, RenderingEngine* _renderingEngine);

	void SetUniformi(const std::string& _uniformName, int _value);
	void SetUniformf(const std::string& _uniformName, float _value);
	void SetUniformMatrix4f(const std::string& _uniformName, const Matrix4f& _value);
	void SetUniformVector3f(const std::string& _uniformName, const Vector3f& _value);
protected:
private:
	static std::map<std::string, ShaderData*> s_resourceMap;

	ShaderData* shaderData;
	std::string fileName;

	void SetUniformDirectionalLight(const std::string& _uniformName, const DirectionalLight& _value);
	void SetUniformPointLight(const std::string& _uniformName, const PointLight& _value);
	void SetUniformSpotLight(const std::string& _uniformName, const SpotLight& _value);
};

#endif
