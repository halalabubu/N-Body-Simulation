#include "ArrayMap.h"
#include <iostream>
void sortArrayMap::sort() {


	//TODO ez to divide for multithreading
	for (size_t i = 0; i < P_COUNT; i++)
	{
		int x = static_cast<int>(particles[i].position.x / partSize);
		int y = static_cast<int>(particles[i].position.y / partSize);

		//std::cout << "X: " << x << " " << "Y: " << y << "\n";
		int index = x + y * ARRAY_LENGTH;
		//std::cout <<"index: " << index << "\n";
		Node* temp;
		if (head[index].next == nullptr)
		{
			head[index].next = new Node;
			head[index].next->part = &particles[i];
			head[index].totalMass = particles[i].mass;
			head[index].com = particles[i].position;
		}
		else
		{
			temp = new Node;
			temp->next = head[index].next;
			temp->part = &particles[i];
			head[index].next = temp;
			head[index].totalMass = head[index].totalMass + head[index].next->part->mass;
			temp = nullptr;

			//head[index].com




		}

	}
}

void sortArrayMap::clear()
{
	//TODO multithread delete, this a thing on cuda?
	//or advanced make all node certain memmory range then just clear that range
	Node* current;
	Node* next;
	for (size_t i = 0; i < MAX_QUADRANTS; i++)
	{
		if (head[i].next != nullptr) {

			next = head[i].next->next;
			current = head[i].next;

			while (next != nullptr) {
				//next = current->next;
				delete current;
				current = next;
				next = current->next;
			}
			delete current;
			delete next;
		}

		head[i].next = nullptr;
		head[i].totalMass = 0;

	}


}

Head* sortArrayMap::getArrayMap()
{
	return head;
}




sortArrayMap::sortArrayMap(Particle ap[])
{
	particles = ap;
	partSize = SUBRANGE / static_cast<float>(ARRAY_LENGTH);
	head = new Head[MAX_QUADRANTS];
}

sortArrayMap::~sortArrayMap()
{
}

