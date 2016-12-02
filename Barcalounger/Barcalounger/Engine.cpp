#include "Engine.h"
#include "time.h"
#include "window.h"
#include "input.h"
#include "renderUtil.h"
#include "util.h"
#include "game.h"
#include "sdl_backend.h"
#include <stdio.h>

CoreEngine* CoreEngine::instance = NULL;
Window *WindowInstance = NULL;
Input *InputInstance = NULL;
CoreEngine *CoreEngine::getInstance() {
	if (instance == NULL) {
		instance = new CoreEngine();
	}
	return instance;
}


CoreEngine::CoreEngine() {
	WindowInstance = Window::getInstance();
	InputInstance = Input::getInstance();
}

void CoreEngine::SetAttributes(int width, int height, double frameRate, Game* game) {
	m_isRunning = false;
	m_width = width;
	m_height = height;
	m_frameTime = (1.0 / frameRate);
	m_game = game;
	m_renderingEngine = NULL;
	
	
	m_game->SetEngine(this);
}

CoreEngine::~CoreEngine()
{
	WindowInstance->Dispose();
	if (m_renderingEngine) delete m_renderingEngine;
}



bool CoreEngine::CreateWindowWithAll(int width, int height, const std::string& gameName) {
	RenderUtil::InitGraphics();
	m_renderingEngine = new RenderingEngine();
	return WindowInstance->CreateWindowWithAll(width, height, gameName);
}

bool CoreEngine::CreateWindowWithName(std::string& gameName)
{
	return CreateWindowWithAll(m_width, m_height, gameName);
}

bool CoreEngine::CreateWindowWithDim(int width, int height) {
	return CreateWindowWithAll(width, height, "Barcalouger Engine");
}

bool CoreEngine::CreateWindowDefault() {
	return CreateWindowWithAll(m_width, m_height, "Barcalounger Engine");
}

void CoreEngine::Start()
{
	if (m_isRunning)
		return;

	Run();
}

void CoreEngine::Stop()
{
	if (!m_isRunning)
		return;

	m_isRunning = false;
}

void CoreEngine::Run()
{
	m_isRunning = true;

	m_game->Init();

	double lastTime = Time::GetTime();
	double unprocessedTime = 0;
	double frameCounter = 0;
	int frames = 0;

	while (m_isRunning)
	{
		bool render = false;

		double startTime = Time::GetTime();
		double passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		if (frameCounter >= 1.0)
		{
			printf("%i\n", frames);
			frames = 0;
			frameCounter = 0;
		}

		while (unprocessedTime > m_frameTime)
		{
			render = true;
			
			//Close if escape is pressed
			SDLSetIsCloseRequested(InputInstance->getESCPressed());
			if (SDLGetIsCloseRequested()) {
				Stop();
			}
			//Update input
			SDL_Event SDLEvent;
			while (SDL_PollEvent(&SDLEvent)) {
				switch (SDLEvent.type) {
				case SDL_KEYDOWN:
					InputInstance->keyDown(SDLEvent);
					break;
				case SDL_KEYUP:
					InputInstance->keyUp(SDLEvent);
					break;
				}
			}

			m_game->Input((float)m_frameTime);
			m_game->Update((float)m_frameTime);

			unprocessedTime -= m_frameTime;
		}

		if (render)
		{
			m_game->Render(m_renderingEngine);
			WindowInstance->Render();
			frames++;
		}
		else
		{
			Util::Sleep(1);
		}
	}
}







/*#include "Engine.h"
#ifndef NULL
#define NULL 0
#endif // !NULL

//Engine is a singleton class
Engine *Engine::instance = NULL;

Engine::Engine()
{
	//once the engine is created, a reference of each class that is
	//needed is brought into the engine
	inputInstance = Input::getInstance();
	renderInstance = Renderer::getInstance();
	logInstance = LogManager::getInstance();
	clockInstance = Clock::getInstance();
}


Engine::~Engine()
{
	//ALL POINTERS NEED TO BE DESTROYED HERE
	//OR IN A DESTROY METHOD
}

//Creates only one instance of the engine in case the
//main program tries to create multiple instances
Engine *Engine::getEngine() {
	if (instance == NULL) {
		instance = new Engine();
	}
	return instance;
}

//Runs through the game loop until the escape button is pressed
void Engine::Run() {
	//sets the initial time in the clock
	clockInstance->init();
	
	//escape is initially set to false and is used to see if the 
	//game should be run
	while (!inputInstance->getESCPressed())
	{
		float temp = clockInstance->getTimeDiffInNanoSeconds();
		//Updates all the keyboard inputs that are handled
		SDL_Event SDLEvent;
		while (SDL_PollEvent(&SDLEvent)) {
			switch (SDLEvent.type) {
			case SDL_KEYDOWN:
				inputInstance->keyDown(SDLEvent);
				break;
			case SDL_KEYUP:
				inputInstance->keyUp(SDLEvent);
				break;
			}
		}

		renderInstance->Render();
		//Renderinstance->clear();
	}
}

void Engine::Destroy() {
	
}

bool Engine::initializeWindowDefault() {
	return renderInstance->initializeWindowDefault();
}
bool Engine::initializeWindow(int width, int height) {
	return renderInstance->initializeWindow(width, height);
}

void Engine::preRender() {
	clockInstance->Update();
}

void Engine::render() {
	renderInstance->Render();
}

void Engine::postRender() {
	//I feel like we don't really need this :/
}
*/




