#pragma once
#include <string>
#include <vector>

#include"Color.h"
#include"Vec2.h"

class Texture
{
	int textureWidth{}; //verbose name to (hopefully) prevent confusion with screenWidth
	int textureHeight{};

	std::vector<Color> pixels; 
public: 
	Texture() = delete; 
	/*Fills 2D std::vector of pixels*/
	Texture(const std::string& imageFilename); 

	/*Maps a UV coordinate to a color (based on pixels member var)*/
	Color sample(const Vec2& uv) const;

private: 
	void flatten(const std::vector<std::vector<Color>>& pixels2D);
};

