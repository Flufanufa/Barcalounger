
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
	Renderer *Renderinstance = Renderer::getInstance();
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

	loginstance->error("testing loading a model");

	

	//if (modelManagerInstance->loadasset("../Assets/Earth1Batman/")) {
	//	loginstance->error("model failed to load");
	//}
	//else {
	//	loginstance->error("model loaded! .... ?????????????");
	//}
	engineInstance->Start();
	loginstance->error("engine ran \"Successfully\"");
	return 0;
}


