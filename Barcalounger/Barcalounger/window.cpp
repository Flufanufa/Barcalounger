#include "window.h"
#include <SDL.h>
#include <GL/glew.h>
#include "sdl_backend.h"
#include "LogManager.h"

int Window::s_width = 0;
int Window::s_height = 0;
std::string Window::s_title = "Title";


Window* Window::instance = NULL;
Window* Window::getInstance() {
	//checks to see if an instance of the renderer exists yet
	//if it doesn't, then it creates one and returns it
	if (instance == NULL) {
		instance = new Window();
	}
	return instance;
}

bool Window::CreateWindowWithAll(int _width, int _height, const std::string& _title)
{
	Window::s_width = _width;
	Window::s_height = _height;
	Window::s_title = _title;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDLCreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, false);

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		LogManager::getInstance()->error("FATAL ERROR: glew did not initialize");
		return false;
	}
	return true;
}

void Window::Render()
{
	SDLSwapBuffers();
}

void Window::Dispose()
{
	SDLDestroyWindow();
	SDL_Quit();
}

bool Window::IsCloseRequested()
{
	return SDLGetIsCloseRequested();
}

void Window::SetFullScreen(bool _value)
{
	SDLSetWindowFullscreen(_value);
}

int Window::GetWidth()
{
	return s_width;
}

int Window::GetHeight()
{
	return s_height;
}

float Window::GetAspect()
{
	return (float)GetWidth() / (float)GetHeight();
}

const std::string& Window::GetTitle()
{
	return s_title;
}
