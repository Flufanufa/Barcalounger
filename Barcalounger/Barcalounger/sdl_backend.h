#ifndef SDL_BACKEND_H
#define SDL_BACKEND_H

bool SDLGetIsCloseRequested();
void SDLSetIsCloseRequested(bool _value);
void SDLCreateWindow(const char* _title, int _x, int _y, int _width, int _height, bool _fullscreen);
void SDLSetWindowFullscreen(bool _value);
void SDLSwapBuffers();
void SDLDestroyWindow();
void SDLSetMousePosition(int _x, int _y);

#endif