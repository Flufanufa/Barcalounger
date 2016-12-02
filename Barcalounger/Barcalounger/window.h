#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <string>

class Window
{
public:
	static Window* getInstance();


	bool CreateWindowWithAll(int width, int height, const std::string& gameName);
	void Render();
	void Dispose();

	bool IsCloseRequested();
	int GetWidth();
	int GetHeight();
	float GetAspect();
	const std::string& GetTitle();

	void SetFullScreen(bool value);

	
protected:
private:
	static Window *instance;

	static int s_width;
	static int s_height;
	static std::string s_title;


	


};

#endif
