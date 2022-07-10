#include "Kernal.cuh"


//int index = blockIdx.x * blockDim.x + threadIdx.x;
//int stride = blockDim.x * gridDim.x;
//for (int i = index; i < n; i += stride)
  //  int x;

__global__ void setTextureColor(int n, sf::Uint8* pixels)
{
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int stride = blockDim.x * gridDim.x;
	for (int i = index; i < n; i += stride)
		pixels[i] = 0x2F;
}




//place particles into array
__global__ void placeIntoPixel(int n, int pixelLength, Particle* particle, Pixel* pixel) {
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int stride = blockDim.x * gridDim.x;
	for (int i = index; i < n; i += stride)

	{

		int indexX = particle[i].pos.x;
		int indexY = particle[i].pos.y;

		//TODO allow multiple particles in the same place
		//pixel[indexX + indexY * pixelLength].particles = new Particle;
		pixel[indexX + indexY * pixelLength].particles = &particle[i];
		pixel[indexX + indexY * pixelLength].totalMass += particle[i].mass;

	}




}
//place particles into array
__global__ void placeIntoGrid(int n, int gridLength, float gridSize, Particle* particle, Grid* grid) {
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int stride = blockDim.x * gridDim.x;
	for (int i = index; i < n; i += stride)
	{

		int indexX = particle[i].pos.x / gridSize;
		int indexY = particle[i].pos.y / gridSize;

		//TODO mass is assumed to be 1 fix later?
		//TODO allow multiple particles in the same place
		//grid[indexX + indexY * gridLength].com.x = particle[i];
		grid[indexX + indexY * gridLength].totalMass += particle[i].mass;



	}
}

__global__ void updateTexture(int n, sf::Uint8* pixels, Pixel* pixel) {

	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int stride = blockDim.x * gridDim.x;
	for (int i = index; i < n; i += stride)
	{
		if (pixel[i].totalMass > 0)
		{
			//make red
			pixels[i * 4] = 0xFF;	//R
			pixels[i * 4+1] = 0xFF;	//G
			pixels[i * 4+2] = 0xFF;	//B
			pixels[i * 4+3] = 0xFF;	//A
		}
		else
		{
			//make black
			pixels[i * 4] = 0x0F;
			pixels[i * 4+1] = 0x0F;
			pixels[i * 4+2] = 0x0F;
			pixels[i * 4+3] = 0xFF;//all alpha is always 0xFF
		}

	}

}
