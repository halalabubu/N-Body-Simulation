#ifndef __KERNAL
#define __KERNAL
#pragma once

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <curand.h>

//__managed__ float MaxDist = 100;
//__managed__ float DampValue = 0.00001;

struct Node
{
	int startIndex;
	int endIndex;
	float2 com;
	float totalMass = 0;

	float2 mins;
	float width;
	Node* parent;
	Node *quadrants[4];
	bool isLeaf = false;
};

struct NodeList {
	Node level0;
	Node level1[4];
	Node level2[16];
	Node level3[64];
	Node level4[256];
	Node level5[1024];
	Node level6[4096];
	Node level7[16384];
	Node level8[65536];
};




struct Particle
{
	float mass = 0;
	float2 pos;
	float2 velocity;
	Node* node;
};




//each pixel is given a struct that contains allparticles within its bounds





//set all pixels to this color
__global__ void setTextureColor(int n, sf::Uint8* pixels);
__global__ void buildTree(
	Particle* particles,Node* node,NodeList *nodeList, int currentLevel, int levels, float width, float xmin, float ymin);
__global__ void setVelSetPos(Particle* particles, NodeList* nlist, float delta);
__global__ void naiveAccel(int n, Particle* particles);



#endif // __KERNAL


