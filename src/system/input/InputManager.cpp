#include "InputManager.h"

InputManager::InputManager()
{
}

void InputManager::pressKey(unsigned int keyId)
{
	keys[keyId] = true;
}

void InputManager::releaseKey(unsigned int keyId)
{
	keys[keyId] = false;
}

void InputManager::pressMouseKey(unsigned int keyId)
{
	mouses[keyId] = true;
}

void InputManager::releaseMouseKey(unsigned int keyId)
{
	mouses[keyId] = false;
}

bool InputManager::isKeyPressed(unsigned int keyId)
{
	auto keyIterator = keys.find(keyId);
	if (keyIterator != keys.end())
	{
		return keyIterator->second;
	}

	return false;
}

bool InputManager::isMouseKeyPressed(unsigned int keyId)
{
	auto keyIterator = mouses.find(keyId);
	if (keyIterator != mouses.end())
	{
		return keyIterator->second;
	}

	return false;
}

MousePosition InputManager::GetMousePosition()
{
	return mousePosition;
}

void InputManager::SetMousePosition( int x, int y )
{	
	mousePosition.previousX = mousePosition.x;
	mousePosition.previousY = mousePosition.y;

	mousePosition.x = x;
	mousePosition.y = y;
}

void InputManager::ResetMousePosition( int x, int y )
{
	mousePosition.x = x;
	mousePosition.previousX = x;

	mousePosition.y = y;
	mousePosition.previousY = y;
}
