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
	static constexpr int TARGET_FPS = 3; 
	static constexpr std::chrono::milliseconds frameDelay{ 1000 / TARGET_FPS };

	SDL_Window* pWindow = nullptr; 
	SDL_Renderer* pRenderer = nullptr;

	int width{}, height{};
	int frameCount{};

public: 
	SDLWrapper(int width, int height); 
	/*@brief Alter "Cube" to Drawable or somesuch in the future
	* @param camera -> this probably ought to mutable (if allowing user input)
	*/
	int run(const Cube& cube, Camera& camera, const std::vector<Vec4>& localVerts, 
		const std::vector<Color>& colors);
	/*Prevents memory leak if exception throws and bypasses SDL calling the quit*/
	~SDLWrapper();

private: 
	SDL_AppResult init(); 

	/*The beast*/
	SDL_AppResult iterate(const Cube& cube, Camera& camera, const std::vector<Vec4>& localVerts, 
		const std::vector<Color>& colors);

	/*Handle event might update camera*/
	SDL_AppResult handleEvent(SDL_Event* pEvent, Camera& camera);

	void quit(SDL_AppResult /*no arg name?*/);

	void draw(const std::unordered_map<Vec2, Color>& rasteredPixels) const;

	/*advances frameCount member var by 1 after frameDelay is reached*/
	void advanceFrame(const std::chrono::steady_clock::time_point& frameStart);
};

