/*
//NO LONGER BEING USED
#ifndef WINDOW_H
#define WINDOW_H
#pragma once
#include <OpenGL\include\glew.h>
#include <SDL.h>

class Window
{
public:
	~Window();
	
	static Window* getInstance();
	
	SDL_Window *getWindow();



	bool initializeWindowDefault();
	bool initializeWindow(int width, int height);

	void Shutdown();
	void ClearRenderer() const;

	


private:
	Window();
	static Window *instance;
	SDL_Rect winRect;

	SDL_Window* SDLwindow;
	SDL_Renderer* SDLrenderer;
	SDL_GLContext glContext;
	
};

#endif // !WINDOW_H
*/