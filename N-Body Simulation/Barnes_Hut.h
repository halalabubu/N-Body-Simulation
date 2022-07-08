#pragma once
#include "Particle.h"
#include "ArrayMap.h"

//make a struct, less on new quad contsruction
//delete pointers for every new frame
//struct Branch {
//	Branch* NW = nullptr;
//	Branch* NE = nullptr;
//	Branch* SE = nullptr;
//	Branch* SW = nullptr;
//
//	sf::Vector2f centerOfMass;
//	float totalMass;
//};



class quadTree
{
public:
	quadTree(sf::RenderWindow* window);
	~quadTree();
	void buildNodes(Head* head);
	void buildQuadrant(Head* head,int Xmin,int Ymin,int size);
private:


	sf::Vertex line[2];
	sf::RenderWindow* window;




	//void buildLeaves();
	//void buildNode(sf::Vector2f* pos, sf::Vector2f origin, float size);
	//void traverseTree();




};






//class B_Hut
//{
//public:
//	B_Hut(sf::Window* window, sf::Vector2f size);
//	~B_Hut(); //delete array? or does it auto?
//
//
//private:
//	sf::Window* window;
//
//	sf::Vector2f size;
//};
//
//B_Hut::B_Hut(sf::Window* window, sf::Vector2f size)
//{
//	this->window = window;
//	this->size = size;
//}
//
//B_Hut::~B_Hut()
//{
//}


