/* 
//NO LONGER BEING USED
#include "Renderer.h"
#include "LogManager.h"

LogManager *loginstance = NULL;
//The renderer is a singleton class
Renderer* Renderer::instance = NULL;

Renderer::Renderer(){
	loginstance = LogManager::getInstance();
	windowInstance = Window::getInstance();
	glEnable(GL_DOUBLEBUFFER);
	if (glGenVertexArrays == NULL) {
		loginstance->setLogLevel(loginstance->LOG_ERROR);
		loginstance->error("genVertexArrays gon fucked up");
	}
	if (glGenBuffers == NULL) {
		loginstance->error("genBuffers dun gon fucked up 2");
	}
	//glGenVertexArrays(1, &vao);
	//glGenBuffers(1, &vbo);
}


Renderer::~Renderer(){
	clear();
}


Renderer* Renderer::getInstance() {
	//checks to see if an instance of the renderer exists yet
	//if it doesn't, then it creates one and returns it
	if (instance == NULL) {
		instance = new Renderer();
	}
	return instance;
}

void Renderer::Render() {
	//Clears the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//windowInstance->ClearRenderer();
	resizeBuffers();

	//The first thing is how the vertices are 
	//grouped up when they are drawn
	//The second is the position in the arrays
	//that it will start from
	//The third is how many points it will use
	glDrawArrays(GL_TRIANGLES, 0, numVertices);


	//ensures the above gl Functions are executed by the 
	//rendering engine asap
	glFlush();
}

//CALL THIS METHOD IF YOU ADD/REMOVE ANYTHING FROM THE LIST OF VERTICES
//^I basically said fuck it so now it gets called each time render is called
void Renderer::resizeBuffers() {

	if (!listOfColorData.empty() && !listOfVertices.empty()) {
		glBindVertexArray(vao);
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(listOfVertices), &listOfVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(0);
	
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(listOfColorData), &listOfColorData, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(1);
	}
}

//we get a list and add it to the one we send to the buffer
void Renderer::addListOfVertices(std::list<glm::vec3> verts) {
	for each (glm::vec3 v in verts) {
		listOfVertices.push_back(v);
		numVertices++;
	}
}
//same thing
void Renderer::addListOfColorData(std::list<glm::vec4> cData) {
	for each (glm::vec4 v in cData) {
		listOfColorData.push_back(v);
	}
}
//clears the lists of vertices and colours
//preferably called by whatever check to see 
//what is in the scene
void Renderer::clear() {
	listOfVertices.clear();
	listOfColorData.clear();
	numVertices = 0;
}

/
bool Renderer::initializeWindowDefault() {
	return windowInstance->CreateWindowDefault();
}

bool Renderer::initializeWindow(int width, int height) {
	return windowInstance->CreateWindowWithDim(width, height);
}
*/
