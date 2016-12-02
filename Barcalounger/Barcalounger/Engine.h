#ifndef COREENGINE_H
#define COREENGINE_H

#include "renderingEngine.h"
#include <string>
#include "window.h"

class Game;

class CoreEngine
{
public:
	static CoreEngine *getInstance();

	
	~CoreEngine();

	bool CreateWindowWithAll(int width, int height, const std::string& gameName);
	bool CreateWindowWithDim(int width, int height);
	bool CreateWindowWithName(std::string &gameName);
	bool CreateWindowDefault();

	void Start();
	void Stop();

	void SetAttributes(int width, int height, double frameRate, Game* game);

	inline RenderingEngine* GetRenderingEngine() { return m_renderingEngine; }
protected:
private:

	static CoreEngine *instance; 
	CoreEngine();

	
	CoreEngine(const CoreEngine& other) {}
	void operator=(const CoreEngine& other) {}

	void Run();

	bool m_isRunning;
	int m_width;
	int m_height;
	double m_frameTime;
	Game* m_game;
	RenderingEngine* m_renderingEngine;
};

#endif // COREENGINE_H























/*#ifndef ENGINE_H
#define ENGINE_H
#pragma once
#include "Input.h"
#include "Renderer.h"
#include "LogManager.h"
#include "Clock.h"

class Engine
{
public:
	static Engine *getEngine();
	void Run();
	void Destroy();
	~Engine();
	bool initializeWindowDefault();
	bool initializeWindow(int width, int height);

	void preRender();
	void render();
	void postRender();

private:
	static Engine *instance;
	Engine();
	Input *inputInstance;
	LogManager *logInstance;
	Renderer *renderInstance;
	Clock *clockInstance;
};

#endif //ENGINE_H*/