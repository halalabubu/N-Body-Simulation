#pragma once
#include "GlobalVariables.h"
#include "Particle.h"

struct Node
{
	Particle* part = nullptr;
	Node* next = nullptr;
};
struct Head {
	Node* next = nullptr;
	int totalMass = 0;
	sf::Vector2f com;//center of mass
};


class sortArrayMap
{
public:
	sortArrayMap(Particle ap[]);
	~sortArrayMap();

	void sort();
	void clear();
	Head* getArrayMap();

private:
	Head* head;
	Particle* particles;
	float partSize;
};

