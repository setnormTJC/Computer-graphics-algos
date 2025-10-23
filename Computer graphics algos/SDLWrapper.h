#pragma once
#define _USE_MATH_DEFINES
#include <SDL3/SDL.h>

#include "Camera.h"
#include "Cube.h"
#include "Mesh.h"
#include <thread>


class SDLWrapper
{
private: 
	static constexpr int TARGET_FPS = 1; 
	static constexpr std::chrono::milliseconds FRAME_DELAY{ 1000 / TARGET_FPS };

	SDL_Window* pWindow = nullptr; 
	SDL_Renderer* pRenderer = nullptr;

	int width{}, height{};
	int frameCount{};




public: 
	SDLWrapper(int width, int height); 
	/*Alter "Cube" to Drawable or somesuch in the future*/
	int run(Cube& cube, Camera& camera, const std::vector<Vec4>& localVerts);
	/*Prevents memory leak if exception throws and bypasses SDL calling the quit*/
	~SDLWrapper();

private: 
	SDL_AppResult init(); 

	/*The beast*/
	SDL_AppResult iterate(Cube& cube, Camera& camera, const std::vector<Vec4>& localVerts);

	SDL_AppResult handleEvent(SDL_Event* pEvent);

	void quit(SDL_AppResult /*no arg name?*/);
};

