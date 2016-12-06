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
	//InputInstance = Input::getInstance();
}

void CoreEngine::SetAttributes(int _width, int _height, double _frameRate, Game* _game) {
	isRunning = false;
	width = _width;
	height = _height;
	frameTime = (1.0 / _frameRate);
	game = _game;
	renderingEngine = NULL;
	
	
	game->SetEngine(this);
}

CoreEngine::~CoreEngine()
{
	WindowInstance->Dispose();
	if (renderingEngine) delete renderingEngine;
}

bool CoreEngine::CreateWindowWithAll(int _width, int _height, const std::string& _gameName) {
	bool isInitialized = WindowInstance->CreateWindowWithAll(_width, _height, _gameName);
	RenderUtil::InitGraphics();
	renderingEngine = new RenderingEngine();
	
	return isInitialized;
}

bool CoreEngine::CreateWindowWithName(std::string& _gameName)
{
	return CreateWindowWithAll(width, height, _gameName);
}

bool CoreEngine::CreateWindowWithDim(int _width, int _height) {
	return CreateWindowWithAll(_width, _height, "Barcalouger Engine");
}

bool CoreEngine::CreateWindowDefault() {
	return CreateWindowWithAll(width, height, "Barcalounger Engine");
}

void CoreEngine::Start()
{
	if (isRunning)
		return;

	Run();
}

void CoreEngine::Stop()
{
	if (!isRunning)
		return;

	isRunning = false;
}

void CoreEngine::Run()
{
	isRunning = true;

	game->Init();

	double lastTime = Time::GetTime();
	double unprocessedTime = 0;
	double frameCounter = 0;
	int frames = 0;

	while (isRunning)
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

		while (unprocessedTime > frameTime)
		{
			render = true;

			if (WindowInstance->IsCloseRequested())
				Stop();

			Input::Update();

			game->Input((float)frameTime);
			game->Update((float)frameTime);

			unprocessedTime -= frameTime;
		}

		if (render)
		{
			game->Render(renderingEngine);
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




