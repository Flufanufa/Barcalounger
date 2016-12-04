#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <string>

class Window
{
public:
	static Window* getInstance();


	bool CreateWindowWithAll(int _width, int _height, const std::string& _gameName);
	void Render();
	void Dispose();

	bool IsCloseRequested();
	int GetWidth();
	int GetHeight();
	float GetAspect();
	const std::string& GetTitle();

	void SetFullScreen(bool _value);

	
protected:
private:
	static Window *instance;

	static int s_width;
	static int s_height;
	static std::string s_title;


	


};

#endif
