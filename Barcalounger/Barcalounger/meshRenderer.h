#ifndef MESHRENDERER_H_INCLUDED
#define MESHRENDERER_H_INCLUDED

#include "gameComponent.h"
#include "ModelMesh.h"
#include "material.h"
#include "shader.h"

class MeshRenderer : public GameComponent
{
public:
	MeshRenderer(Mesh* _mesh, Material* _material)
	{
		mesh = _mesh;
		material = _material;
	}

	virtual ~MeshRenderer()
	{
		if (mesh) delete mesh;
		if (material) delete material;
	}

	virtual void Render(Shader* _shader, RenderingEngine* _renderingEngine)
	{
		_shader->Bind();
		_shader->UpdateUniforms(GetTransform(), *material, _renderingEngine);
		mesh->Draw();
	}
protected:
private:
	Mesh* mesh;
	Material* material;
};

#endif // MESHRENDERER_H_INCLUDED