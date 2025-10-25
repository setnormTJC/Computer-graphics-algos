#pragma once
#include <array>
#include<algorithm>
#include<iostream>
#include<map> 
#include<tuple> //for std::tie
#include <vector>

#include"Edge.h"
#include"Vec2.h"
#include"Vec4.h" //used for triangle area calculation (I defined cross product in Vec4)


class Triangle
{
private:
	std::array<Vec2, 3> vertices{};
	bool isFlatBottom{};
	bool isFlatTop{}; 


	/*extrema for scanline algo (and drawing box)*/
	int xMin = INT_MAX; 
	int xMax = INT_MIN; 
	int yMin = INT_MAX;
	int yMax = INT_MIN; 

public: 
	Triangle() = delete;
	Triangle(const std::array<Vec2, 3>& vertices);
	/*construct an EQUILATERAL triangle given one edge*/
	Triangle(const Edge& equilateralEdge);

	/*@brief Scanline algo! Hooray!
	* @param screenWidth -> nonintuitive inclusion BUT necessary because this algo CLAMPS based on screen dims
	*/
	std::vector<Vec2> getPointsThatFillTriangle(int screenWidth, int screenHeight) const;

	std::vector<Vec2> getPointsThatOutlineTriangle() const; 

	Box2D getBoundingBoxDimensions() const; 
	std::array<Edge, 3> getEdges() const; 
	std::array<Vec2, 3> getVertices() const; 

	/*The area of a triangle is HALF the magnitude of the cross product of two sides of the triangle(with sides constructed as Vec4s)*/
	float getArea() const; 
private: 
	/*For geometric funsies (nerd)*/
	float getAngleOfAdjacentEdges(const int indexOfFirstEdge, const int indexOfSecondEdge) const;
	/*modifies the member variable vertices such that v[0].y <= v[1].y <= v[2].y*/
	void sortVertices();

	std::vector<Vec2> getPointsThatFillFlatBottomTriangle(int screenWidth, int screenHeight) const;
	std::vector<Vec2> getPointsThatFillFlatTopTriangle(int screenWidth, int screenHeight) const;
	
};




