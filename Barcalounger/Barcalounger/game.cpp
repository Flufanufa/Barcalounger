#include "game.h"
#include "renderingEngine.h"
#include <iostream>
#include "meshRenderer.h"

void Game::Input(float _delta)
{
	root.InputAll(_delta);
}

void Game::Update(float _delta)
{
	root.UpdateAll(_delta);
}

void Game::Render(RenderingEngine* _renderingEngine)
{
	_renderingEngine->Render(&root);
}

void Game::Init() {
	//THIS IS WHERE ALL THE GAME OBJECTS WERE INITIALLY LOADED

}