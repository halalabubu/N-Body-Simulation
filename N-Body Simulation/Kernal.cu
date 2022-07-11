#include "Kernal.cuh"

//int index = blockIdx.x * blockDim.x + threadIdx.x;
//int stride = blockDim.x * gridDim.x;
//for (int i = index; i < n; i += stride)
  //  int x;


__global__ void buildTree(
	Particle* particles, Node* node, NodeList* nList, int currentLevel, int maxlevels, float width, float xmin, float ymin) {
	//we will only ever have 1 thread per funtion call :( but will have thousands of calls

	node->mins.x = xmin;
	node->mins.y = ymin;
	node->width = width;

	// if levels = 0 return or 1 particle set mass and com return
	//TODO
	//edit this if we want to set min parts before sub

	//node->quadrants = new Node[4];

	if (node->startIndex == node->endIndex)
	{
		node->com = particles[node->startIndex].pos;
		node->totalMass = particles[node->startIndex].mass;
		return;
	}
	if (currentLevel == maxlevels)
	{
		float2 comT; comT.x = 0; comT.y = 0;
		float totalmass = 0;
		for (size_t i = node->startIndex; i < node->endIndex; i++)
		{
			comT.x = particles[i].pos.x * particles[i].mass;
			comT.y = particles[i].pos.y * particles[i].mass;
			totalmass += particles[i].mass;
		}
		node->com.x = comT.x / totalmass;
		node->com.y = comT.y / totalmass;
		node->totalMass = totalmass;
		return;
	}
	//we need to init head node values
	Particle temp;

	int qEnd0;
	int qEnd2;
	int qEnd3 = node->endIndex;//the end node is the last node
	int qEnd1 = node->endIndex;

	int partcount[4] = { 0,0,0,0 };

	// sort quad 0,1 on left and 2,3 on right
	// also gives us qEnd1 value;
	for (size_t i = node->startIndex; i < qEnd1; i)
	{
		if (particles[i].pos.y > ymin + width / 2)
		{
			temp = particles[qEnd1];
			particles[qEnd1] = particles[i];
			particles[i] = temp;
			qEnd1--;
		}
		else
			i++;
	}

	// calc qEnd0
	qEnd0 = qEnd1;
	for (size_t i = node->startIndex; i < qEnd0; i)
	{
		if (particles[i].pos.x >= xmin + width / 2)
		{
			temp = particles[qEnd0];
			particles[qEnd0] = particles[i];
			particles[i] = temp;
			qEnd0--;
			partcount[1]++;
		}
		else
		{
			i++;
			partcount[0]++;
		}
	}

	//do the same for bot side
	// calc qEnd2
	qEnd2 = qEnd3;
	for (size_t i = qEnd1; i < qEnd2; i)
	{
		if (particles[i].pos.x >= xmin + width / 2)
		{
			temp = particles[qEnd2];
			particles[qEnd2] = particles[i];
			particles[i] = temp;
			qEnd2--;
			partcount[3]++;
		}
		else
		{
			i++;
			partcount[2]++;
		}
	}

	//cudaMallocManaged(node->quadrants, 4*sizeof(Node));
	//quadrants = new Node[4];











	//set values for the 4 new quadrants
	int tindex = node->startIndex;
	for (size_t i = 0; i < 4; i++)
	{
		if (partcount[i] > 0)
		{
			node->quadrants[i]->startIndex = tindex;
			node->quadrants[i]->endIndex = node->quadrants[i]->startIndex + partcount[i] - 1;
			tindex = node->quadrants[i]->endIndex + 1;


			//quadrants[i].startIndex = tindex;
			//quadrants[i].endIndex = quadrants[i].startIndex + partcount[i] - 1;
			//tindex = quadrants[i].endIndex;
		}
		else
			node->quadrants[i]->totalMass == 0;
	}


	//before new function calls
	currentLevel++;
	//build tree call here

	//buildTree <<<1, 1 >>> (particles,&nList->level0,nList, CURRENT_LEVEL, LEVELS, GRIDSIZE, 0, 0);

	buildTree << <1, 1 >> > (particles, node->quadrants[0], nList, currentLevel, maxlevels, width / 2, xmin, ymin);
	buildTree << <1, 1 >> > (particles, node->quadrants[1], nList, currentLevel, maxlevels, width / 2, xmin + width / 2, ymin);
	buildTree << <1, 1 >> > (particles, node->quadrants[2], nList, currentLevel, maxlevels, width / 2, xmin, ymin + width / 2);
	buildTree << <1, 1 >> > (particles, node->quadrants[3], nList, currentLevel, maxlevels, width / 2, xmin + width / 2, ymin + width / 2);

	// wait for these to finish
	cudaDeviceSynchronize();


	//calc mass and com here
	node->totalMass = 0;
	float totalX = 0, totalY = 0;

	for (size_t i = 0; i < 4; i++)
	{
		if (node->quadrants[i] != nullptr)
		{
			node->totalMass += node->quadrants[i]->totalMass;
			totalX += node->quadrants[i]->com.x * node->quadrants[i]->totalMass;
			totalY += node->quadrants[i]->com.y * node->quadrants[i]->totalMass;
		}
	
	}
	node->com.x = totalX / node->totalMass;
	node->com.y = totalY / node->totalMass;

}

//__global__ void

//void calcAccel();
//void calcVelocity

