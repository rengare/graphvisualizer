#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "../config/AppConfig.h"

static int WINDOW_FLAGS = SDL_WINDOW_OPENGL |SDL_WINDOW_RESIZABLE;

class Renderer
{
public:
	Renderer();
	Renderer(AppConfig config);
	void SetGlAttributes();
	SDL_GLContext GetRendererContext();
	SDL_Window* GetWindow();

	void BeginDraw();
	void Draw( GLuint vao );
	void EndDraw();

private:

private:
	SDL_GLContext rendererContext;
	SDL_Window* window;

	int VSYNC;
};


#endif