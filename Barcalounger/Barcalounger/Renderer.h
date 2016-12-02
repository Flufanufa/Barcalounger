#ifndef renderer
#define renderer
#pragma once
#include <vgl.h>
#include <glm\glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <list>
#include "Window.h"
#include "SDL.h"

class Renderer
{
private:
	Renderer();
	static Renderer *instance;
	//used for both the vertices and the colour data
	unsigned int numVertices = 0;

	enum VAO_IDs { Triangles, NumVAOs };
	enum Buffer_IDs { ArrayBuffer, NumBuffers };
	enum Attrib_IDs { vPosition = 0 };

	GLuint VAOs[NumVAOs];
	GLuint Buffers[NumBuffers];
	GLuint location;
	
	GLuint vao, vbo;

	std::list<glm::vec3> listOfVertices;
	std::list<glm::vec4> listOfColorData;

	void resizeBuffers();


	SDL_Window *SDLwindow;
	Window *windowInstance;


public:
	static Renderer* getInstance();
	~Renderer();
	void Render();
	
	void addListOfVertices(std::list<glm::vec3> verts);
	void addListOfColorData(std::list<glm::vec4> cData);
	//clears the lists and arrays
	void clear();
	bool initializeWindowDefault();
	bool initializeWindow(int width, int height);

};

#endif // !renderer