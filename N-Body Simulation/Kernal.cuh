#ifndef __KERNAL
#define __KERNAL

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <curand.h>


struct Node
{
	int startIndex;
	int endIndex;
	float2 com;
	float totalMass;

	float2 mins;
	float width;

	Node *quadrants[4];
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
	float mass;
	float2 pos;
	float2 velocity;
};




//each pixel is given a struct that contains allparticles within its bounds





//set all pixels to this color
__global__ void setTextureColor(int n, sf::Uint8* pixels);
__global__ void buildTree(
	Particle* particles,Node* node,NodeList *nodeList, int currentLevel, int levels, float width, float xmin, float ymin);





#endif // __KERNAL


