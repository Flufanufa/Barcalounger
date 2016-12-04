
#include <vgl.h>
#include <glm\glm.hpp>
#include "Engine.h"
#include "LogManager.h"
#include "ModelManager.h"
#include "Renderer.h"
#include "game.h"
#include "ModelMesh.h"
#include "meshRenderer.h"
#undef main

int main(int argc, char* argv[]) {
	//SDL_Init(SDL_INIT_EVERYTHING);
	//Renderer *Renderinstance = Renderer::getInstance();
	LogManager *loginstance = LogManager::getInstance();
	CoreEngine *engineInstance = CoreEngine::getInstance();
	ModelManager *modelManagerInstance = ModelManager::getInstance();

	
	loginstance->setLogLevel(loginstance->LOG_ERROR);
	loginstance->error("instances load");

	Game game;
	
	//SET ATTRIBUTES NEEDS TO BE CALLED FIRST AND NEEDS AN INSTANCE OF A GAME PASSED THROUGH
	engineInstance->SetAttributes(800, 450, 80, &game);
	loginstance->error("Attributes set");
	engineInstance->CreateWindowDefault();
	loginstance->error("window opened - no errors");


	////////////////////////////////////////////////////////////////////////////////
	//everything from here to the next line of /'s is to add objects to the scene///
	
	//a game object must be created for each object you want to add to the scene
	//this also includes lights
	GameObject* monkey = new GameObject();
	GameObject* directionalLightObject = new GameObject();


	//ShaderData::ShaderData("forward-directional");

	monkey->AddComponent(new MeshRenderer(new Mesh("../resources/models/monkey3.obj"), new Material(new Texture("test.png"), 1, 8)));
	monkey->GetTransform().SetPos(Vector3f(0, -1, 5));
	monkey->GetTransform().SetScale(2.0f);
	//Creates a camera in the monkey model//
	monkey->AddChild((new GameObject())->AddComponent(new Camera(Matrix4f().InitPerspective(ToRadians(70.0f), 1.777777f, 0.1f, 1000.0f)))); ////1.7777 is the aspect ratio for 800 / 450 - figure out why window isn't returning it properly


	directionalLightObject->AddComponent(new DirectionalLight(Vector3f(1, 1, 1), 0.4f));
	directionalLightObject->GetTransform().LookAt(Vector3f(0, 0, 0), Vector3f(0, 1, 0));

	game.AddToScene(monkey);
	game.AddToScene(directionalLightObject);

	directionalLightObject->GetTransform().SetRot(Quaternion(Vector3f(1, 0, 0), ToRadians(-45)));

	////////////////////////////////////////////////////////////////////////////////

	//engine will loop
	engineInstance->Start();
	loginstance->error("Game closed with no errors");
	return 0;
}


