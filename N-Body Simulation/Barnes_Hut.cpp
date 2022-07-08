#include "Barnes_Hut.h"
#include <iostream>
//#include <thread>




void quadTree::buildNodes(Head* head)
{
	int Xmin = 0;//origins
	int Ymin = 0;


	buildQuadrant(head, Xmin, Ymin, ARRAY_LENGTH);
}

void quadTree::buildQuadrant(Head* head, int Xmin, int Ymin, int size)
{
	if (size == 1 )// max sub level at 
	{
		//std::cout << "mass: " << ARRAY_LENGTH / size << "\n";
		return;

	}
	//lol, close enough
	float pF = SUBRANGE / ARRAY_LENGTH;
	float full = pF * size;
	float half = full / 2.0f;

	//top down line
	line[0] = sf::Vector2f(Xmin * pF + half, Ymin * pF);
	line[1] = sf::Vector2f(Xmin * pF + half, Ymin * pF + full);
	window->draw(line, 2, sf::Lines);
	line[0] = sf::Vector2f(Xmin * pF, Ymin * pF + half);
	line[1] = sf::Vector2f(Xmin * pF + full, Ymin * pF + half);
	window->draw(line, 2, sf::Lines);

	int mass = 0;

	for (size_t x = Xmin; x < Xmin +size /2; x++)
	{
		for (size_t y = Ymin; y < Ymin + size / 2; y++)
		{
			mass += head[x + y * ARRAY_LENGTH].totalMass;
		}
	}
	if (mass > 0)
	{
		//std::cout << "mass: " << mass << "\n";
		buildQuadrant(head, Xmin, Ymin, size / 2);
	}

	mass = 0;
	for (size_t x = Xmin + size / 2; x < Xmin + size; x++)
	{
		for (size_t y = Ymin; y < Ymin + size / 2; y++)
		{
			mass += head[x + y * ARRAY_LENGTH].totalMass;
		}
	}
	if (mass > 0)
	{
		buildQuadrant(head, Xmin + size / 2, Ymin, size / 2);
	}

	mass = 0;
	for (size_t x = Xmin; x < Xmin + size/2; x++)
	{
		for (size_t y = Ymin+size/2; y < Ymin + size ; y++)
		{
			mass += head[x + y * ARRAY_LENGTH].totalMass;
		}
	}
	if (mass > 0)
	{
		buildQuadrant(head, Xmin , Ymin + size / 2, size / 2);
	}
	mass = 0;
	for (size_t x = Xmin+size/2; x < Xmin + size; x++)
	{
		for (size_t y = Ymin + size / 2; y < Ymin + size; y++)
		{
			mass += head[x + y * ARRAY_LENGTH].totalMass;
		}
	}
	if (mass > 0)
	{
		buildQuadrant(head, Xmin + size / 2, Ymin + size / 2, size / 2);
	}








}






quadTree::quadTree(sf::RenderWindow* window)
{
	this->window = window;
}

quadTree::~quadTree()
{
}

//quadTree::quadTree(sf::RenderWindow* window)
//{
//
//	nodeHead = new Node;
//	nodePtr = nullptr;
//
//	this->window = window;
//
//	////top down line
//	//line[0] = sf::Vector2f(origin.x + size / 2.0f, origin.y);
//	//line[1] = sf::Vector2f(origin.x + size / 2.0f, origin.y + size);
//	//window->draw(line, 2, sf::Lines);
//	////left rigth line
//	//line[0] = sf::Vector2f(origin.x, origin.y + size / 2);
//	//line[1] = sf::Vector2f(origin.x + size, origin.y + size / 2);
//	//window->draw(line, 2, sf::Lines);
//}
//
//quadTree::~quadTree()
//{
//
//}
////pos,origin,size
//void buildNode(Node* node, sf::Vector2f* pos, sf::Vector2f origin, float size)
//{
////	int length = sizeof(pos) / sizeof(pos[0]);
////	float xAVG = 0;
////	float yAVG = 0;
////	for (size_t i = 0; i < length; i++)
////	{
////		xAVG = xAVG + pos[i].x;
////		yAVG = yAVG + pos[i].y;
////		node->totalMass++;//mass is 1
////	}
////	xAVG = xAVG / node->totalMass;
////	yAVG = yAVG / node->totalMass;
////	node->centerOfMass = sf::Vector2f(xAVG, yAVG);
////
////	node->size = size;
////	node->origin = origin;
//}
//void buildLeaves() {
//
//}
//
//
//
//void quadrantCheck(Particle* particles, Particle* newPList, int points, int& pInQ, float x, float y, float size)
//{
//	//for (size_t i = 0; i < points; i++)
//	//{
//	//	//NW quadrant
//	//	if (
//	//		particles[i].getpPosition().x < x + size &&
//	//		particles[i].getpPosition().x > x &&
//	//		particles[i].getpPosition().y > y &&
//	//		particles[i].getpPosition().y < y + size
//	//		) {
//	//		//std::cout << particles[i].getpPosition().x<<"\n";
//	//		newPList[pInQ] = particles[i];
//	//		pInQ++;
//	//	}
//	//}
//
//
//}
//
//
//void quadTree::buildLeaves() {
//	//num of points in quadrant
//	//int NWp = 0;
//	//int NEp = 0;
//	//int SEp = 0;
//	//int SWp = 0;
//	//float hSize = size / 2.0f;
//	//give each own thread
//
//	//std::thread thread1(quadrantCheck, particles, partNWTemp, points, std::ref(NWp), origin.x, origin.y, hSize);
//	//std::thread thread2(quadrantCheck, particles, partNETemp, points, std::ref(NEp), origin.x + hSize, origin.y, hSize);
//	//std::thread thread3(quadrantCheck, particles, partSETemp, points, std::ref(SEp), origin.x + hSize, origin.y + hSize, hSize);
//	//std::thread thread4(quadrantCheck, particles, partSWTemp, points, std::ref(SWp), origin.x, origin.y + hSize, hSize);
//	//thread1.join(); thread2.join(); thread3.join(); thread4.join();
//
//	//reduce part array after each check
//	//quadrantCheck(particles, partNWTemp, points, NWp, origin.x, origin.y, hSize);
//	//quadrantCheck(particles, partNETemp, points, NEp, origin.x + hSize, origin.y, hSize);
//	//quadrantCheck(particles, partSETemp, points, SEp, origin.x + hSize, origin.y + hSize, hSize);
//	//quadrantCheck(particles, partSWTemp, points, SWp, origin.x, origin.y + hSize, hSize);
//
//
//	//NWp = index;
//	//index = 0;
//
//
//
////	std::cout
////		<< "nwp: " << NWp
////		<< "nep: " << NEp
////		<< "sep: " << SEp
////		<< "swp: " << SWp
////		<< std::endl;
//	//now create leafNode if part count is >1
//	//if (NWp > 1)
//	//	NW = new quadTree(partNWTemp, NWp, origin, hSize, window);
//	//if (NEp > 1)
//	//	NE = new quadTree(partNETemp, NEp, sf::Vector2f(origin.x + hSize, origin.y), hSize, window);
//	//if (SEp > 1)
//	//	SE = new quadTree(partSETemp, SEp, sf::Vector2f(origin.x + hSize, origin.y + hSize), hSize, window);
//	//if (SWp > 1)
//	//	SW = new quadTree(partSWTemp, SWp, sf::Vector2f(origin.x, origin.y + hSize), hSize, window);
//
//
//
//
//}


