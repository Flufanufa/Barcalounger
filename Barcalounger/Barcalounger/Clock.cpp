#include "Clock.h"
Clock *Clock::instance = NULL;
Clock::Clock() {
	
}

Clock::~Clock() {

}

Clock *Clock::getInstance() {
	if (instance == NULL) {
		instance = new Clock();
	}
	return instance;
}

void Clock::init()
{
	currentTime = previousTime = std::chrono::high_resolution_clock::now();
}

void Clock::Update() {
	previousTime = currentTime;
	currentTime = std::chrono::high_resolution_clock::now();
}

float Clock::getTimeDiffInNanoSeconds() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count();
}