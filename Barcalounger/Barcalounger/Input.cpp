#include "input.h"
#include <SDL.h>
#include "sdl_backend.h"

const static int NUM_KEYS = 512;
const static int NUM_MOUSEBUTTONS = 256;

static SDL_Event e;

static int mouseX = 0;
static int mouseY = 0;

static bool inputs[NUM_KEYS];
static bool downKeys[NUM_KEYS];
static bool upKeys[NUM_KEYS];

static bool mouseInput[NUM_MOUSEBUTTONS];
static bool downMouse[NUM_MOUSEBUTTONS];
static bool upMouse[NUM_MOUSEBUTTONS];

void Input::Update()
{
	for (int i = 0; i < NUM_MOUSEBUTTONS; i++)
	{
		downMouse[i] = false;
		upMouse[i] = false;
	}

	for (int i = 0; i < NUM_KEYS; i++)
	{
		downKeys[i] = false;
		upKeys[i] = false;
	}

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			SDLSetIsCloseRequested(true);

		if (e.type == SDL_MOUSEMOTION)
		{
			mouseX = e.motion.x;
			mouseY = e.motion.y;
		}

		if (e.type == SDL_KEYDOWN)
		{
			int value = e.key.keysym.scancode;

			inputs[value] = true;
			downKeys[value] = true;
		}
		if (e.type == SDL_KEYUP)
		{
			int value = e.key.keysym.scancode;

			inputs[value] = false;
			upKeys[value] = true;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			int value = e.button.button;

			mouseInput[value] = true;
			downMouse[value] = true;
		}
		if (e.type == SDL_MOUSEBUTTONUP)
		{
			int value = e.button.button;

			mouseInput[value] = false;
			upMouse[value] = true;
		}
	}
}

bool Input::GetKey(int keyCode)
{
	return inputs[keyCode];
}

bool Input::GetKeyDown(int keyCode)
{
	return downKeys[keyCode];
}

bool Input::GetKeyUp(int keyCode)
{
	return upKeys[keyCode];
}

bool Input::GetMouse(int button)
{
	return mouseInput[button];
}

bool Input::GetMouseDown(int button)
{
	return downMouse[button];
}

bool Input::GetMouseUp(int button)
{
	return upMouse[button];
}

Vector2f Input::GetMousePosition()
{
	Vector2f res((float)mouseX, (float)mouseY);
	return res;
}

void Input::SetCursor(bool visible)
{
	if (visible)
		SDL_ShowCursor(1);
	else
		SDL_ShowCursor(0);
}

void Input::SetMousePosition(Vector2f pos)
{
	SDLSetMousePosition((int)pos.GetX(), (int)pos.GetY());
}


/*
#include "Input.h"
#ifndef NULL
#define NULL 0
#endif // !NULL


Input *Input::instance = NULL;
Input::Input()
{
}

Input *Input::getInstance() {
	if (instance == NULL) {
		instance = new Input();
	}
	return instance;
}

Input::~Input()
{
}
//Changes the state of each key once they are released
//should be changed back to false
void Input::keyUp(SDL_Event key) {
	switch (key.key.keysym.sym)
	{
	case 'a': case 'A': aPressed = !aPressed; break;
	case 'b': case 'B': bPressed = !bPressed; break;
	case 'c': case 'C': cPressed = !cPressed; break;
	case 'd': case 'D': dPressed = !dPressed; break;
	case 'e': case 'E': ePressed = !ePressed; break;
	case 'f': case 'F': fPressed = !fPressed; break;
	case 'g': case 'G': gPressed = !gPressed; break;
	case 'h': case 'H': hPressed = !hPressed; break;
	case 'i': case 'I': iPressed = !iPressed; break;
	case 'j': case 'J': jPressed = !jPressed; break;
	case 'k': case 'K': kPressed = !kPressed; break;
	case 'l': case 'L': lPressed = !lPressed; break;
	case 'm': case 'M': mPressed = !mPressed; break;
	case 'n': case 'N': nPressed = !nPressed; break;
	case 'o': case 'O': oPressed = !oPressed; break;
	case 'p': case 'P': pPressed = !pPressed; break;
	case 'q': case 'Q': qPressed = !qPressed; break;
	case 'r': case 'R': rPressed = !rPressed; break;
	case 's': case 'S': sPressed = !sPressed; break;
	case 't': case 'T': tPressed = !tPressed; break;
	case 'u': case 'U': uPressed = !uPressed; break;
	case 'v': case 'V': vPressed = !vPressed; break;
	case 'w': case 'W': wPressed = !wPressed; break;
	case 'x': case 'X': xPressed = !xPressed; break;
	case 'y': case 'Y': yPressed = !yPressed; break;
	case 'z': case 'Z': zPressed = !zPressed; break;
	case 27 : escPressed = !escPressed; break;
	default:
		break;
	}
}

//Changes the state of each key that is pressed
//each button will become true when they are pressed
void Input::keyDown(SDL_Event key) {
	switch (key.key.keysym.sym)
	{
	case 'a': case 'A': aPressed = true; break;
	case 'b': case 'B': bPressed = true; break;
	case 'c': case 'C': cPressed = true; break;
	case 'd': case 'D': dPressed = true; break;
	case 'e': case 'E': ePressed = true; break;
	case 'f': case 'F': fPressed = true; break;
	case 'g': case 'G': gPressed = true; break;
	case 'h': case 'H': hPressed = true; break;
	case 'i': case 'I': iPressed = true; break;
	case 'j': case 'J': jPressed = true; break;
	case 'k': case 'K': kPressed = true; break;
	case 'l': case 'L': lPressed = true; break;
	case 'm': case 'M': mPressed = true; break;
	case 'n': case 'N': nPressed = true; break;
	case 'o': case 'O': oPressed = true; break;
	case 'p': case 'P': pPressed = true; break;
	case 'q': case 'Q': qPressed = true; break;
	case 'r': case 'R': rPressed = true; break;
	case 's': case 'S': sPressed = true; break;
	case 't': case 'T': tPressed = true; break;
	case 'u': case 'U': uPressed = true; break;
	case 'v': case 'V': vPressed = true; break;
	case 'w': case 'W': wPressed = true; break;
	case 'x': case 'X': xPressed = true; break;
	case 'y': case 'Y': yPressed = true; break;
	case 'z': case 'Z': zPressed = true; break;
	case 27 : escPressed = true; break;
	default:
		break;
	}
}
//Each of these return true if the key is pressed
//and false if it isn't
bool Input::getAPressed() { return aPressed; }
bool Input::getBPressed() { return bPressed; }
bool Input::getCPressed() { return cPressed; }
bool Input::getDPressed() { return dPressed; }
bool Input::getEPressed() { return ePressed; }
bool Input::getFPressed() { return fPressed; }
bool Input::getGPressed() { return gPressed; }
bool Input::getHPressed() { return hPressed; }
bool Input::getIPressed() { return iPressed; }
bool Input::getJPressed() { return jPressed; }
bool Input::getKPressed() { return kPressed; }
bool Input::getLPressed() { return lPressed; }
bool Input::getMPressed() { return mPressed; }
bool Input::getNPressed() { return nPressed; }
bool Input::getOPressed() { return oPressed; }
bool Input::getPPressed() { return pPressed; }
bool Input::getQPressed() { return qPressed; }
bool Input::getRPressed() { return rPressed; }
bool Input::getSPressed() { return sPressed; }
bool Input::getTPressed() { return tPressed; }
bool Input::getUPressed() { return uPressed; }
bool Input::getVPressed() { return vPressed; }
bool Input::getWPressed() { return wPressed; }
bool Input::getXPressed() { return xPressed; }
bool Input::getYPressed() { return yPressed; }
bool Input::getZPressed() { return zPressed; }
//Used explicitly for closing the game - for now - eventually there will 
//be a bool that is used only for the quit button in one of the HUDs
bool Input::getESCPressed() { return escPressed; }
*/