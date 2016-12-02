#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

class Clock
{
private:
	Clock();
	static Clock *instance;
	//nanoseconds?
	std::chrono::steady_clock::time_point currentTime;
	std::chrono::steady_clock::time_point previousTime;
	
public:
	void init();
	void Update();
	static Clock *getInstance();
	float getTimeDiffInNanoSeconds();
	~Clock();
};

#endif