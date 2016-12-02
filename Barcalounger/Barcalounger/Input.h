#ifndef Input_H
#define Input_H
#pragma once
#include "SDL\Include\SDL.h"
class Input
{
public:
	static Input *getInstance();
	~Input();
	void keyUp(SDL_Event SDLevent);
	void keyDown(SDL_Event SDLevent);

private:
	static Input *instance;
	Input();
	bool aPressed = false;
	bool bPressed = false;
	bool cPressed = false;
	bool dPressed = false;
	bool ePressed = false;
	bool fPressed = false;
	bool gPressed = false;
	bool hPressed = false;
	bool iPressed = false;
	bool jPressed = false;
	bool kPressed = false;
	bool lPressed = false;
	bool mPressed = false;
	bool nPressed = false;
	bool oPressed = false;
	bool pPressed = false;
	bool qPressed = false;
	bool rPressed = false;
	bool sPressed = false;
	bool tPressed = false;
	bool uPressed = false;
	bool vPressed = false;
	bool wPressed = false;
	bool xPressed = false;
	bool yPressed = false;
	bool zPressed = false;
	bool escPressed = false;


public:
	bool getAPressed();
	bool getBPressed();
	bool getCPressed();
	bool getDPressed();
	bool getEPressed();
	bool getFPressed();
	bool getGPressed();
	bool getHPressed();
	bool getIPressed();
	bool getJPressed();
	bool getKPressed();
	bool getLPressed();
	bool getMPressed();
	bool getNPressed();
	bool getOPressed();
	bool getPPressed();
	bool getQPressed();
	bool getRPressed();
	bool getSPressed();
	bool getTPressed();
	bool getUPressed();
	bool getVPressed();
	bool getWPressed();
	bool getXPressed();
	bool getYPressed();
	bool getZPressed();
	bool getESCPressed();

};

#endif // !Input
