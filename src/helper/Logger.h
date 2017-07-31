#ifndef VISUALIZER_LOGGER_H
#define VISUALIZER_LOGGER_H

#include <cstdlib>

#include <iostream>
#include <SDL2/SDL.h>

using namespace std;
class Logger
{
public:
	static void LogError(char* message)
	{
		Log(message);
		getchar();

		SDL_Quit();
		exit(69);
	};

	static void LogError(char* message, char* fileName)
	{
		Log(message, fileName);
		getchar();

		SDL_Quit();
		exit(69);
	};

	static void Log(char* message)
	{
		cout << "message: ";
		cout << message << endl;
		cout << "===========" << endl;
	};

	static void Log(char* message, char* fileName)
	{
		cout << "Log in file: ";
		cout << fileName << endl;
		cout << "message: ";
		cout << message << endl;
		cout << "===========" << endl;
	};

	static void Log(float number)
	{
		cout << "Number: ";
		cout << number << endl;
	};
};


#endif
