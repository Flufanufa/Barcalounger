#include "game.h"
#include "renderingEngine.h"
#include <iostream>
#include "meshRenderer.h"

void Game::Input(float delta)
{
	m_root.InputAll(delta);
}

void Game::Update(float delta)
{
	m_root.UpdateAll(delta);
}

void Game::Render(RenderingEngine* renderingEngine)
{
	renderingEngine->Render(&m_root);
}

void Game::Init() {
	GameObject* BATMAN = new GameObject();
	GameObject* directionalLightObject = new GameObject();


	//ShaderData::ShaderData("forward-directional");
	BATMAN->AddComponent(new MeshRenderer(new Mesh("../resources/models/monkey3.obj"), new Material(new Texture("test.png"), 1, 8)));
	BATMAN->GetTransform().SetPos(Vector3f(0, -1, 5));
	BATMAN->GetTransform().SetScale(2.0f);
	BATMAN->AddChild((new GameObject())
		->AddComponent(new Camera(Matrix4f().InitPerspective(ToRadians(70.0f), 1.777777f, 0.1f, 1000.0f))));
	
	
	directionalLightObject->AddComponent(new DirectionalLight(Vector3f(1, 1, 1), 0.4f));
	directionalLightObject->GetTransform().LookAt(Vector3f(0, 0, 0), Vector3f(0, 1, 0));

	AddToScene(BATMAN);
	AddToScene(directionalLightObject);



	directionalLightObject->GetTransform().SetRot(Quaternion(Vector3f(1, 0, 0), ToRadians(-45)));
}