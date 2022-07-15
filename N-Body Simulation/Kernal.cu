#include "Kernal.cuh"



__global__ void buildTree(
	Particle* particles, Node* node, NodeList* nList, int currentLevel, int maxlevels, float width, float xmin, float ymin) {
	//we will only ever have 1 thread per funtion call :( but will have thousands of calls

	node->mins.x = xmin;
	node->mins.y = ymin;
	node->width = width;
	node->isLeaf = false;
	// if levels = 0 return or 1 particle set mass and com return
	//TODO
	//edit this if we want to set min parts before sub

	//node->quadrants = new Node[4];
	int minParts = 10;
	//if (node->startIndex == node->endIndex)
	if (node->endIndex - node->startIndex < minParts)
	{
		float2 comT; comT.x = 0; comT.y = 0;
		float totalmass = 0;
		for (size_t i = node->startIndex; i <= node->endIndex; i++)
		{
			comT.x = particles[i].pos.x * particles[i].mass;
			comT.y = particles[i].pos.y * particles[i].mass;
			totalmass += particles[i].mass;
			particles[i].node = node;
		}
		node->com.x = comT.x / totalmass;
		node->com.y = comT.y / totalmass;
		node->totalMass = totalmass;
		for (size_t i = 0; i < 4; i++) {
			node->quadrants[i]->totalMass = 0;
			node->quadrants[i]->isLeaf = false;
		}
		node->isLeaf = true;
		return;
	}
	if (currentLevel == maxlevels)
	{
		float2 comT; comT.x = 0; comT.y = 0;
		float totalmass = 0;
		for (size_t i = node->startIndex; i <= node->endIndex; i++)
		{
			comT.x = particles[i].pos.x * particles[i].mass;
			comT.y = particles[i].pos.y * particles[i].mass;
			totalmass += particles[i].mass;
			particles[i].node = node;
		}
		node->com.x = comT.x / totalmass;
		node->com.y = comT.y / totalmass;
		node->totalMass = totalmass;
		node->isLeaf = true;
		return;
	}
	//we need to init head node values
	Particle temp;

	int qEnd0;
	int qEnd2;
	int qEnd3 = node->endIndex;//the end node is the last node
	int qEnd1 = node->endIndex;

	int partcount[4] = { 0,0,0,0 };
	int topCount = 0;
	int botCount = 0;

	// sort quad 0,1 on left and 2,3 on right
	// also gives us qEnd1 value;
	for (int i = node->startIndex; i <= qEnd1; i)
	{
		if (particles[i].pos.y > ymin + width / 2)
		{
			temp = particles[qEnd1];
			particles[qEnd1] = particles[i];
			particles[i] = temp;
			qEnd1--;
			botCount++;
		}
		else {
			i++;
			topCount++;
		}
	}

	// calc qEnd0
	qEnd0 = node->startIndex + topCount - 1;
	if (topCount > 0)
	{

		for (int i = node->startIndex; i <= qEnd0; i)
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
	}


	//do the same for bot side
	// calc qEnd2
	qEnd2 = node->endIndex;
	qEnd1 = node->startIndex + topCount;


	if (botCount > 0)
	{
		for (int i = qEnd1; i <= qEnd2; i)
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
			node->quadrants[i]->totalMass = 0;
	}


	//before new function calls
	currentLevel++;
	//build tree call here

	//buildTree <<<1, 1 >>> (particles,&nList->level0,nList, CURRENT_LEVEL, LEVELS, GRIDSIZE, 0, 0);



	if (partcount[0] > 0)
		buildTree << <1, 1 >> > (particles, node->quadrants[0], nList, currentLevel, maxlevels, width / 2, xmin, ymin);
	else
		node->quadrants[0]->totalMass = 0;

	if (partcount[1] > 0)
		buildTree << <1, 1 >> > (particles, node->quadrants[1], nList, currentLevel, maxlevels, width / 2, xmin + width / 2, ymin);
	else
		node->quadrants[1]->totalMass = 0;

	if (partcount[2] > 0)
		buildTree << <1, 1 >> > (particles, node->quadrants[2], nList, currentLevel, maxlevels, width / 2, xmin, ymin + width / 2);
	else
		node->quadrants[2]->totalMass = 0;

	if (partcount[3] > 0)
		buildTree << <1, 1 >> > (particles, node->quadrants[3], nList, currentLevel, maxlevels, width / 2, xmin + width / 2, ymin + width / 2);
	else
		node->quadrants[3]->totalMass = 0;





	// wait for these to finish
	cudaDeviceSynchronize();


	//calc mass and com here
	node->totalMass = 0;
	float totalX = 0, totalY = 0;

	for (size_t i = 0; i < 4; i++)
	{
		//if (node->quadrants[i] != nullptr)
		//{
		node->totalMass += node->quadrants[i]->totalMass;
		totalX += node->quadrants[i]->com.x * node->quadrants[i]->totalMass;
		totalY += node->quadrants[i]->com.y * node->quadrants[i]->totalMass;
		//}

	}
	node->com.x = totalX / node->totalMass;
	node->com.y = totalY / node->totalMass;

}




//int index = blockIdx.x * blockDim.x + threadIdx.x;
//int stride = blockDim.x * gridDim.x;
//for (int i = index; i < n; i += stride)
//int x;

// we dont need to sqrt since the accel form requires r^2 (2D)
__device__ float getDistance(float2& a, float2& b) {
	return  pow(b.x - a.x, 2) + pow(b.y - a.y, 2);
}
__device__ float2 getAccel(float2& a, float2& b, float& mass) {
	float2 accel;
	float dist = getDistance(a, b);
	accel.x = mass * (b.x - a.x) / (dist + 0.00001f);
	accel.y = mass * (b.y - a.y) / (dist + 0.00001f);

	return  accel;
}

__device__ float2 getallAccel(float2& pPos, Node* node, Particle* particles)
{
	float2 accel, temp;
	accel.x = 0.0f;
	accel.y = 0.0f;
	for (size_t i = node->startIndex; i <= node->endIndex; i++)
	{

			temp = getAccel(pPos, particles[i].pos, particles[i].mass);
			accel.x += temp.x;
			accel.y += temp.y;
		

	}

	return accel;
}




__device__ float2 checkChildren(float2& pPos, Node* currentNode, Particle* particles) {
	float2 accel;
	float2 temp;
	accel.x = 0;
	accel.y = 0;
	for (size_t i = 0; i < 4; i++)
	{
		if (currentNode->quadrants[i]->totalMass > 0)
		{
			float dist = getDistance(pPos, currentNode->quadrants[i]->com);
			if (dist > 22500)
			{
				temp = getAccel(pPos, currentNode->quadrants[i]->com, currentNode->quadrants[i]->totalMass);
				accel.x += temp.x;
				accel.y += temp.y;
			}
			else if (currentNode->quadrants[i]->isLeaf == true)
			{
				temp = getallAccel(pPos, currentNode->quadrants[i], particles);
				accel.x += temp.x;
				accel.y += temp.y;
			}
			else
			{
				temp = checkChildren(pPos, currentNode->quadrants[i], particles);
				accel.x += temp.x;
				accel.y += temp.y;
			}

		}


	}
	return accel;
}

__device__ float2 traverseNode(float2& pPos, Node* currentNode, Particle* particles) {
	//check for if at top
	float2 accel;
	float2 temp;
	accel.x = 0;
	accel.y = 0;
	Node* prevNode;
	//TODO
	//calc for original node parts
	temp = getallAccel(pPos, currentNode, particles);
	accel.x += temp.x;
	accel.y += temp.y;


	while (currentNode->parent != nullptr)
	{
		//move up
		//traverseNode(pPos, currentNode->parent, currentNode);
		prevNode = currentNode;
		currentNode = currentNode->parent;
		//look at children
		for (size_t i = 0; i < 4; i++)
		{
			if (currentNode->quadrants[i] != prevNode)
			{
				temp = checkChildren(pPos, currentNode->quadrants[i], particles);
				accel.x += temp.x;
				accel.y += temp.y;
			}

		}



	}


	return accel;
}
__global__ void setVelSetPos(Particle* particles, NodeList* nlist, float delta) {

	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int stride = blockDim.x * gridDim.x;
	float2 temp;
	for (int i = index; i <= nlist->level0.endIndex; i += stride)
	{
		temp = traverseNode(particles[i].pos, particles[i].node, particles);
		particles[i].velocity.x = particles[i].velocity.x + 0.05f * temp.x;
		particles[i].velocity.y = particles[i].velocity.y + 0.05f * temp.y;
		particles[i].pos.x = particles[i].pos.x + particles[i].velocity.x;
		particles[i].pos.y = particles[i].pos.y + particles[i].velocity.y;
	}





}

//void calcAccel();
//void calcVelocity

//__global__ void naive(Particle* particles, NodeList* nlist)