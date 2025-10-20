#pragma once
#include <array>
#include<iostream>
#include<map> 
#include<tuple> //for std::tie
#include <vector>

#include"Edge.h"
#include"Vec2.h"



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

	/*Scanline algo! Hooray!*/
	std::vector<Vec2> getPointsThatFillTriangle() const;

	std::vector<Vec2> getPointsThatOutlineTriangle() const; 

	Box2D getBoundingBoxDimensions() const; 
	std::array<Edge, 3> getEdges() const; 
	std::array<Vec2, 3> getVertices() const; 


private: 
	/*For geometric funsies (nerd)*/
	float getAngleOfAdjacentEdges(const int indexOfFirstEdge, const int indexOfSecondEdge) const;
	/*modifies the member variable vertices such that v[0].y <= v[1].y <= v[2].y*/
	void sortVertices();

	std::vector<Vec2> getPointsThatFillFlatBottomTriangle() const; 		
	std::vector<Vec2> getPointsThatFillFlatTopTriangle() const;
	
};




