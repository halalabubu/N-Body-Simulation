#ifndef __KERNAL
#define __KERNAL

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <curand.h>

struct Particle
{
	float mass;
	float2 pos;
	float2 velocity;
};
//each pixel is given a struct that contains allparticles within its bounds
struct Pixel {
	float totalMass = 0;
	Particle* particles = nullptr;
};
struct Grid {
	float totalMass;
	float2 com;
};


//set all pixels to this color
__global__ void setTextureColor(int n, sf::Uint8* pixels);


//init particle values, then place into array
//__global__ void initParticles(int n, sf::Uint8* pixels);

//calculate acceleration and adjust velocity
//__global__ void calcAccel(int n, sf::Uint8* pixels);

//adjust position
//__global__ void calcPos(int n, sf::Uint8* pixels);

//place particles into array
__global__ void placeIntoPixel(int n, int pixelLength, Particle* particle, Pixel* pixel);
//place particles into array
__global__ void placeIntoGrid(int n, int gridLength, float gridSize, Particle* particle, Grid* grid);

//update pixels
__global__ void updateTexture(int n, sf::Uint8* pixels, Pixel* pixel);




#endif // __KERNAL


