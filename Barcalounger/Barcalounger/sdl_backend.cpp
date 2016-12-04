#include "sdl_backend.h"
#include <SDL.h>
#include <iostream>

static SDL_Window* window;
static SDL_GLContext glContext;
static bool isCloseRequested = false;

bool SDLGetIsCloseRequested()
{
	return isCloseRequested;
}

void SDLSetIsCloseRequested(bool _value)
{
	isCloseRequested = _value;
}

void SDLCreateWindow(const char* _title, int _x, int _y, int _width, int _height, bool _fullscreen)
{
	int mode = 0;

	if (_fullscreen)
		mode = SDL_WINDOW_FULLSCREEN;

	window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL | mode);
	glContext = SDL_GL_CreateContext(window);

	//SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	SDL_GL_SetSwapInterval(1);
}

void SDLSetWindowFullscreen(bool _value)
{
	int mode = 0;
	if (_value)
		mode = SDL_WINDOW_FULLSCREEN;
	else
		mode = 0;

	SDL_SetWindowFullscreen(window, mode);
}

void SDLSwapBuffers()
{
	SDL_GL_SwapWindow(window);
}

void SDLDestroyWindow()
{
	if (!window)
		return;

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
}

void SDLSetMousePosition(int _x, int _y)
{
	SDL_WarpMouseInWindow(window, _x, _y);
}
