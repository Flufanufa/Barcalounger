#include "shader.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <cstdlib>
#include "LogManager.h"
#include "util.h"

//--------------------------------------------------------------------------------
// Variable Initializations
//--------------------------------------------------------------------------------
std::map<std::string, ShaderData*> Shader::s_resourceMap;

//--------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------
static void CheckShaderError(int _shader, int _flag, bool _isProgram, const std::string& _errorMessage);
static std::vector<UniformStruct> FindUniformStructs(const std::string& _shaderText);
static std::string FindUniformStructName(const std::string& _structStartToOpeningBrace);
static std::vector<TypedData> FindUniformStructComponents(const std::string& _openingBraceToClosingBrace);
static std::string LoadShader(const std::string& _fileName);

//--------------------------------------------------------------------------------
// Constructors/Destructors
//--------------------------------------------------------------------------------
ShaderData::ShaderData(const std::string& _fileName)
{
	program = glCreateProgram();

	if (program == 0)
	{
		LogManager::getInstance()->error("Shader failed to load");
		exit(1);
	}

	std::string vertexShaderText = LoadShader(_fileName + ".vs");
	std::string fragmentShaderText = LoadShader(_fileName + ".fs");

	AddVertexShader(vertexShaderText);
	AddFragmentShader(fragmentShaderText);

	AddAllAttributes(vertexShaderText);

	CompileShader();

	AddShaderUniforms(vertexShaderText);
	AddShaderUniforms(fragmentShaderText);
}

ShaderData::~ShaderData()
{
	for (std::vector<int>::iterator it = shaders.begin(); it != shaders.end(); ++it)
	{
		glDetachShader(program, *it);
		glDeleteShader(*it);
	}
	glDeleteProgram(program);
}

Shader::Shader(const std::string& _fileName)
{
	fileName = _fileName;

	std::map<std::string, ShaderData*>::const_iterator it = s_resourceMap.find(_fileName);
	if (it != s_resourceMap.end())
	{
		shaderData = it->second;
		shaderData->AddReference();
	}
	else
	{
		shaderData = new ShaderData(_fileName);
		s_resourceMap.insert(std::pair<std::string, ShaderData*>(_fileName, shaderData));
	}
}

Shader::~Shader()
{
	if (shaderData && shaderData->RemoveReference())
	{
		if (fileName.length() > 0)
			s_resourceMap.erase(fileName);

		delete shaderData;
	}
}

//--------------------------------------------------------------------------------
// Member Function Implementation
//--------------------------------------------------------------------------------
void Shader::Bind()
{
	glUseProgram(shaderData->GetProgram());
}

void Shader::UpdateUniforms(const Transform& _transform, const Material& _material, RenderingEngine* _renderingEngine)
{
	Matrix4f worldMatrix = _transform.GetTransformation();
	Matrix4f projectedMatrix = _renderingEngine->GetMainCamera().GetViewProjection() * worldMatrix;

	for (unsigned int i = 0; i < shaderData->GetUniformNames().size(); i++)
	{
		std::string uniformName = shaderData->GetUniformNames()[i];
		std::string uniformType = shaderData->GetUniformTypes()[i];

		if (uniformType == "sampler2D")
		{
			int samplerSlot = _renderingEngine->GetSamplerSlot(uniformName);
			_material.GetTexture(uniformName)->Bind(samplerSlot);
			SetUniformi(uniformName, samplerSlot);
		}
		else if (uniformName.substr(0, 2) == "T_")
		{
			if (uniformName == "T_MVP")
				SetUniformMatrix4f(uniformName, projectedMatrix);
			else if (uniformName == "T_model")
				SetUniformMatrix4f(uniformName, worldMatrix);
			else
				throw "Invalid Transform Uniform: " + uniformName;
		}
		else if (uniformName.substr(0, 2) == "R_")
		{
			std::string unprefixedName = uniformName.substr(2, uniformName.length());

			if (uniformType == "vec3")
				SetUniformVector3f(uniformName, _renderingEngine->GetVector3f(unprefixedName));
			else if (uniformType == "float")
				SetUniformf(uniformName, _renderingEngine->GetFloat(unprefixedName));
			else if (uniformType == "DirectionalLight")
				SetUniformDirectionalLight(uniformName, *(DirectionalLight*)_renderingEngine->GetActiveLight());
			else if (uniformType == "PointLight")
				SetUniformPointLight(uniformName, *(PointLight*)_renderingEngine->GetActiveLight());
			else if (uniformType == "SpotLight")
				SetUniformSpotLight(uniformName, *(SpotLight*)_renderingEngine->GetActiveLight());
			else
				_renderingEngine->UpdateUniformStruct(_transform, _material, this, uniformName, uniformType);
		}
		else if (uniformName.substr(0, 2) == "C_")
		{
			if (uniformName == "C_eyePos")
				SetUniformVector3f(uniformName, _renderingEngine->GetMainCamera().GetTransform().GetTransformedPos());
			else
				throw "Invalid Camera Uniform: " + uniformName;
		}
		else
		{
			if (uniformType == "vec3")
				SetUniformVector3f(uniformName, _material.GetVector3f(uniformName));
			else if (uniformType == "float")
				SetUniformf(uniformName, _material.GetFloat(uniformName));
			else
				throw uniformType + " is not supported by the Material class";
		}
	}
}

void Shader::SetUniformi(const std::string& _uniformName, int _value)
{
	glUniform1i(shaderData->GetUniformMap().at(_uniformName), _value);
}

void Shader::SetUniformf(const std::string& _uniformName, float _value)
{
	glUniform1f(shaderData->GetUniformMap().at(_uniformName), _value);
}

void Shader::SetUniformVector3f(const std::string& _uniformName, const Vector3f& _value)
{
	glUniform3f(shaderData->GetUniformMap().at(_uniformName), _value.GetX(), _value.GetY(), _value.GetZ());
}

void Shader::SetUniformMatrix4f(const std::string& _uniformName, const Matrix4f& _value)
{
	glUniformMatrix4fv(shaderData->GetUniformMap().at(_uniformName), 1, GL_FALSE, &(_value[0][0]));
}

void Shader::SetUniformDirectionalLight(const std::string& _uniformName, const DirectionalLight& _directionalLight)
{
	SetUniformVector3f(_uniformName + ".direction", _directionalLight.GetTransform().GetTransformedRot().GetForward());
	SetUniformVector3f(_uniformName + ".base.color", _directionalLight.color);
	SetUniformf(_uniformName + ".base.intensity", _directionalLight.intensity);
}

void Shader::SetUniformPointLight(const std::string& _uniformName, const PointLight& _pointLight)
{
	SetUniformVector3f(_uniformName + ".base.color", _pointLight.color);
	SetUniformf(_uniformName + ".base.intensity", _pointLight.intensity);
	SetUniformf(_uniformName + ".atten.constant", _pointLight.atten.constant);
	SetUniformf(_uniformName + ".atten.linear", _pointLight.atten.linear);
	SetUniformf(_uniformName + ".atten.exponent", _pointLight.atten.exponent);
	SetUniformVector3f(_uniformName + ".position", _pointLight.GetTransform().GetTransformedPos());
	SetUniformf(_uniformName + ".range", _pointLight.range);
}

void Shader::SetUniformSpotLight(const std::string& _uniformName, const SpotLight& _spotLight)
{
	SetUniformVector3f(_uniformName + ".pointLight.base.color", _spotLight.color);
	SetUniformf(_uniformName + ".pointLight.base.intensity", _spotLight.intensity);
	SetUniformf(_uniformName + ".pointLight.atten.constant", _spotLight.atten.constant);
	SetUniformf(_uniformName + ".pointLight.atten.linear", _spotLight.atten.linear);
	SetUniformf(_uniformName + ".pointLight.atten.exponent", _spotLight.atten.exponent);
	SetUniformVector3f(_uniformName + ".pointLight.position", _spotLight.GetTransform().GetTransformedPos());
	SetUniformf(_uniformName + ".pointLight.range", _spotLight.range);
	SetUniformVector3f(_uniformName + ".direction", _spotLight.GetTransform().GetTransformedRot().GetForward());
	SetUniformf(_uniformName + ".cutoff", _spotLight.cutoff);
}

void ShaderData::AddVertexShader(const std::string& _text)
{
	AddProgram(_text, GL_VERTEX_SHADER);
}

void ShaderData::AddGeometryShader(const std::string& _text)
{
	AddProgram(_text, GL_GEOMETRY_SHADER);
}

void ShaderData::AddFragmentShader(const std::string& _text)
{
	AddProgram(_text, GL_FRAGMENT_SHADER);
}

void ShaderData::AddProgram(const std::string& _text, int _type)
{
	int shader = glCreateShader(_type);

	if (shader == 0)
	{
		fprintf(stderr, "Error creating shader type %d\n", _type);
		exit(1);
	}

	const GLchar* p[1];
	p[0] = _text.c_str();
	GLint lengths[1];
	lengths[0] = _text.length();

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar InfoLog[1024];

		glGetShaderInfoLog(shader, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shader, InfoLog);

		exit(1);
	}

	glAttachShader(program, shader);
	shaders.push_back(shader);
}

void ShaderData::AddAllAttributes(const std::string& _vertexShaderText)
{
	static const std::string ATTRIBUTE_KEY = "attribute";

	int currentAttribLocation = 0;
	size_t attributeLocation = _vertexShaderText.find(ATTRIBUTE_KEY);
	while (attributeLocation != std::string::npos)
	{
		bool isCommented = false;
		size_t lastLineEnd = _vertexShaderText.rfind(";", attributeLocation);

		if (lastLineEnd != std::string::npos)
		{
			std::string potentialCommentSection = _vertexShaderText.substr(lastLineEnd, attributeLocation - lastLineEnd);
			isCommented = potentialCommentSection.find("//") != std::string::npos;
		}

		if (!isCommented)
		{
			size_t begin = attributeLocation + ATTRIBUTE_KEY.length();
			size_t end = _vertexShaderText.find(";", begin);

			std::string attributeLine = _vertexShaderText.substr(begin + 1, end - begin - 1);

			begin = attributeLine.find(" ");
			std::string attributeName = attributeLine.substr(begin + 1);

			glBindAttribLocation(program, currentAttribLocation, attributeName.c_str());//SetAttribLocation(attributeName, currentAttribLocation);
			currentAttribLocation++;
		}
		attributeLocation = _vertexShaderText.find(ATTRIBUTE_KEY, attributeLocation + ATTRIBUTE_KEY.length());
	}
}

void ShaderData::AddShaderUniforms(const std::string& _shaderText)
{
	static const std::string UNIFORKEY = "uniform";

	std::vector<UniformStruct> structs = FindUniformStructs(_shaderText);

	size_t uniformLocation = _shaderText.find(UNIFORKEY);
	while (uniformLocation != std::string::npos)
	{
		bool isCommented = false;
		size_t lastLineEnd = _shaderText.rfind(";", uniformLocation);

		if (lastLineEnd != std::string::npos)
		{
			std::string potentialCommentSection = _shaderText.substr(lastLineEnd, uniformLocation - lastLineEnd);
			isCommented = potentialCommentSection.find("//") != std::string::npos;
		}

		if (!isCommented)
		{
			size_t begin = uniformLocation + UNIFORKEY.length();
			size_t end = _shaderText.find(";", begin);

			std::string uniformLine = _shaderText.substr(begin + 1, end - begin - 1);

			begin = uniformLine.find(" ");
			std::string uniformName = uniformLine.substr(begin + 1);
			std::string uniformType = uniformLine.substr(0, begin);

			uniformNames.push_back(uniformName);
			uniformTypes.push_back(uniformType);
			AddUniform(uniformName, uniformType, structs);
		}
		uniformLocation = _shaderText.find(UNIFORKEY, uniformLocation + UNIFORKEY.length());
	}
}

void ShaderData::AddUniform(const std::string& _uniformName, const std::string& _uniformType, const std::vector<UniformStruct>& _structs)
{
	bool addThis = true;

	for (unsigned int i = 0; i < _structs.size(); i++)
	{
		if (_structs[i].name.compare(_uniformType) == 0)
		{
			addThis = false;
			for (unsigned int j = 0; j < _structs[i].memberNames.size(); j++)
			{
				AddUniform(_uniformName + "." + _structs[i].memberNames[j].name, _structs[i].memberNames[j].type, _structs);
			}
		}
	}

	if (!addThis)
		return;

	unsigned int location = glGetUniformLocation(program, _uniformName.c_str());

	assert(location != INVALID_VALUE);

	uniformMap.insert(std::pair<std::string, unsigned int>(_uniformName, location));
}

void ShaderData::CompileShader()
{
	glLinkProgram(program);
	CheckShaderError(program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(program);
	CheckShaderError(program, GL_VALIDATE_STATUS, true, "Invalid shader program");
}

//--------------------------------------------------------------------------------
// Static Function Implementations
//--------------------------------------------------------------------------------
static void CheckShaderError(int _shader, int _flag, bool _isProgram, const std::string& _errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (_isProgram)
		glGetProgramiv(_shader, _flag, &success);
	else
		glGetShaderiv(_shader, _flag, &success);

	if (!success)
	{
		if (_isProgram)
			glGetProgramInfoLog(_shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(_shader, sizeof(error), NULL, error);

		fprintf(stderr, "%s: '%s'\n", _errorMessage.c_str(), error);
	}
}

static std::string LoadShader(const std::string& _fileName)
{
	std::ifstream file;
	file.open(("../resources/shaders/" + _fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);

			if (line.find("#include") == std::string::npos)
				output.append(line + "\n");
			else
			{
				std::string includeFileName = Util::Split(line, ' ')[1];
				includeFileName = includeFileName.substr(1, includeFileName.length() - 2);

				std::string toAppend = LoadShader(includeFileName);
				output.append(toAppend + "\n");
			}
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << _fileName << std::endl;
	}

	return output;
};

static std::vector<TypedData> FindUniformStructComponents(const std::string& _openingBraceToClosingBrace)
{
	static const char charsToIgnore[] = { ' ', '\n', '\t', '{' };
	static const size_t UNSIGNED_NEG_ONE = (size_t)-1;

	std::vector<TypedData> result;
	std::vector<std::string> structLines = Util::Split(_openingBraceToClosingBrace, ';');

	for (unsigned int i = 0; i < structLines.size(); i++)
	{
		size_t nameBegin = UNSIGNED_NEG_ONE;
		size_t nameEnd = UNSIGNED_NEG_ONE;

		for (unsigned int j = 0; j < structLines[i].length(); j++)
		{
			bool isIgnoreableCharacter = false;

			for (unsigned int k = 0; k < sizeof(charsToIgnore) / sizeof(char); k++)
			{
				if (structLines[i][j] == charsToIgnore[k])
				{
					isIgnoreableCharacter = true;
					break;
				}
			}

			if (nameBegin == UNSIGNED_NEG_ONE && isIgnoreableCharacter == false)
			{
				nameBegin = j;
			}
			else if (nameBegin != UNSIGNED_NEG_ONE && isIgnoreableCharacter)
			{
				nameEnd = j;
				break;
			}
		}

		if (nameBegin == UNSIGNED_NEG_ONE || nameEnd == UNSIGNED_NEG_ONE)
			continue;

		TypedData newData;
		newData.type = structLines[i].substr(nameBegin, nameEnd - nameBegin);
		newData.name = structLines[i].substr(nameEnd + 1);

		result.push_back(newData);
	}

	return result;
}

static std::string FindUniformStructName(const std::string& _structStartToOpeningBrace)
{
	return Util::Split(Util::Split(_structStartToOpeningBrace, ' ')[0], '\n')[0];
}

static std::vector<UniformStruct> FindUniformStructs(const std::string& _shaderText)
{
	static const std::string STRUCT_KEY = "struct";
	std::vector<UniformStruct> result;

	size_t structLocation = _shaderText.find(STRUCT_KEY);
	while (structLocation != std::string::npos)
	{
		structLocation += STRUCT_KEY.length() + 1; //Ignore the struct keyword and space

		size_t braceOpening = _shaderText.find("{", structLocation);
		size_t braceClosing = _shaderText.find("}", braceOpening);

		UniformStruct newStruct;
		newStruct.name = FindUniformStructName(_shaderText.substr(structLocation, braceOpening - structLocation));
		newStruct.memberNames = FindUniformStructComponents(_shaderText.substr(braceOpening, braceClosing - braceOpening));

		result.push_back(newStruct);
		structLocation = _shaderText.find(STRUCT_KEY, structLocation);
	}

	return result;
}
