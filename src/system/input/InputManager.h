#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <unordered_map>
#include <SDL2/SDL.h>

#include "../../helper/Logger.h"

using namespace std;

struct MousePosition
{
	int x;
	int y;
	int previousX;
	int previousY;
};

class InputManager
{
public:
	InputManager();
	void pressKey(unsigned int keyId);
	void releaseKey(unsigned int keyId);
	void pressMouseKey(unsigned int keyId);
	void releaseMouseKey(unsigned int keyId);
	bool isKeyPressed(unsigned int keyId);

	bool isMouseKeyPressed(unsigned int keyId);
	
	MousePosition GetMousePosition();
	void SetMousePosition( int x, int y);
	void ResetMousePosition( int x, int y );

private:
	unordered_map<unsigned int, bool> keys;
	unordered_map<unsigned int, bool> mouses;
	MousePosition mousePosition;
};

#endif // !INPUT_HANDLER_H
