/*#include "Window.h"

Window* Window::instance = NULL;
Window* Window::getInstance() {
	//checks to see if an instance of the renderer exists yet
	//if it doesn't, then it creates one and returns it
	if (instance == NULL) {
		instance = new Window();
	}
	return instance;
}


Window::Window(){

}


Window::~Window(){

}

SDL_Window *Window::getWindow() {
	return SDLwindow;
}

bool Window::initializeWindowDefault() {
	return initializeWindow(800, 450);
}

bool Window::initializeWindow(int width, int height) {
	
	//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Initialize video subsystem
	//If for some reason something doesn't get created
	//Everything is destroyed and false is returned
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		Shutdown();
		return false;
	}
	else
	{
		// Create window
		SDLwindow = SDL_CreateWindow("Barcalounger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL /*| SDL_WINDOW_SHOWN*//*);
		if (SDLwindow == NULL){
			Shutdown();
			return false;
			// Display error message
			//printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			// Create OpenGL context
			glContext = SDL_GL_CreateContext(SDLwindow);
			if (glContext == NULL) {
				Shutdown();
				return false;
			}
			else {
				SDL_GL_SetSwapInterval(1);
				// Initialize glew
				glewInit();
			}
		}
		return true;
	}
	
	//USELESS???
	//WILL BE LEFT UNTIL THE FINALISH BUILD .... maybe?
	/*winRect.w = width;
	winRect.h = height;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		return false;
	}

	!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	SDLwindow = SDL_CreateWindow("Barcalounger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winRect.w, winRect.h, SDL_WINDOW_SHOWN);
	if (!SDLwindow) {
		Shutdown();
		return false;
	}

	SDLrenderer = SDL_CreateRenderer(SDLwindow, -1, SDL_RENDERER_ACCELERATED);

	if (!SDLrenderer) {
		Shutdown();
		return false;
	}

	SDL_SetRenderDrawColor(SDLrenderer, 100, 100, 100, 255);
	SDL_RenderFillRect(SDLrenderer, &winRect);
	SDL_RenderPresent(SDLrenderer);
	SDLsurface = SDL_GetWindowSurface(SDLwindow);
	ClearRenderer();

	return true;
	*/
	/*
}

void Window::Shutdown() {
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyRenderer(SDLrenderer);
	SDLrenderer = nullptr;
	SDL_DestroyWindow(SDLwindow);
	SDLwindow = nullptr;
	SDL_Quit();
}


void Window::ClearRenderer() const {
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(SDLwindow);
}*/




