#include "util.h"
#include <SDL.h>

void Util::Sleep(int _milliseconds)
{
	SDL_Delay(_milliseconds);
}

std::vector<std::string> Util::Split(const std::string& _s, char _delim)
{
	std::vector<std::string> elems;

	const char* cstr = _s.c_str();
	unsigned int strLength = (unsigned int)_s.length();
	unsigned int start = 0;
	unsigned int end = 0;

	while (end <= strLength)
	{
		while (end <= strLength)
		{
			if (cstr[end] == _delim)
				break;
			end++;
		}

		elems.push_back(_s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elems;
}