#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::Renderer(AppConfig config)
{
	window = SDL_CreateWindow(config.name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		config.width, config.height, WINDOW_FLAGS);

	rendererContext = SDL_GL_CreateContext(window);
	VSYNC = config.isVsyncEnabled;

	SetGlAttributes();
}

void Renderer::SetGlAttributes()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	rendererContext = SDL_GL_CreateContext(window);

	SDL_GL_SetSwapInterval(VSYNC);
	
}

SDL_GLContext Renderer::GetRendererContext()
{
	return rendererContext;
}

SDL_Window * Renderer::GetWindow()
{
	return window;
}

void Renderer::BeginDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw( GLuint vao )
{
	glBindVertexArray( vao );
	glDrawArrays( GL_TRIANGLES, 0, 3 );
}

void Renderer::EndDraw()
{
	SDL_GL_SwapWindow(window);
}
