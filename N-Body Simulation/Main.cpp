#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Particle.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Barnes_Hut.h"
#include "GlobalVariables.h"
#include "ArrayMap.h"


// most performant way to render, use shader to modify texture?
// color using Viscosity ??


//temp nbody equation
sf::Vector2f acceleration(sf::Vector2f object1, sf::Vector2f object2);
sf::Vector2f accelerationSum(Particle particles[], int index);


//without barnes hut 300 brings to 69 fps
//with:
//add toggle so one can switch?
// 




//float G_CONSTANT = 0.001f;

int main()
{
	int VERTICAL_RESOLUTION = 1080;
	int HORIZONTAL_RESOLUTION = 1920;
	float ASPECT_RATIO = 16 / 9;
	float DELTA = 5.0f / 1000.0f;// 
	//float DELTA = 1.0f;



	sf::RenderWindow window(sf::VideoMode(HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION), "N-Body Simulation");
	window.setFramerateLimit(144);

	sf::View camera(sf::Vector2f(130.0f*1.5, 130.0f*1.5), sf::Vector2f(720.0f, 480.f));

	Particle* particles;
	//Particle* particlesSorted;
	sf::CircleShape* circle;

	particles = new Particle[P_COUNT];
	//particlesSorted = new Particle[P_COUNT];
	circle = new sf::CircleShape[P_COUNT];

	sortArrayMap arrayMap(particles);
	quadTree tree(&window);


	for (size_t i = 0; i < P_COUNT; i++)
	{
		circle[i].setRadius(0.5f);
		circle[i].setFillColor(sf::Color(255, 0, 0, 255));
	}


	for (size_t i = 0; i < P_COUNT; i++)
	{
		particles[i].position =
			sf::Vector2f(std::rand() % 360 + 20, std::rand() % 360 + 20);
	}


	for (size_t i = 0; i < P_COUNT; i++)
	{
		//particles[i].setVelocity(sf::Vector2f(std::rand()%40, std::rand() % 40));
	}
	//Tree Build


	// run the program as long as the window is open
	bool moveUp = false;
	bool moveDown = false;
	bool moveLeft = false;
	bool moveRight = false;
	bool zoomIn = false;
	bool zoomOut = false;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				moveUp = true;
			else
				moveUp = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				moveDown = true;
			else
				moveDown = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				moveLeft = true;
			else
				moveLeft = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				moveRight = true;
			else
				moveRight = false;
			//std::cout << sf::Event::MouseWheelScrollEvent::delta;


			//event.mouseWheelScroll.delta
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
				zoomOut = true;
			else
				zoomOut = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
				zoomIn = true;
			else
				zoomIn = false;


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();

			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (moveUp)
			camera.move(0.f, -1.f);
		if (moveDown)
			camera.move(0.f, 1.f);
		if (moveLeft)
			camera.move(-1.f, 0.f);
		if (moveRight)
			camera.move(1.f, 0.f);
		if (zoomIn)
			camera.zoom(0.99f);
		if (zoomOut)
			camera.zoom(1.01f);

		window.setView(camera);
		//quadTree* Tree = new quadTree(particles, P_COUNT, sf::Vector2f(0, 0), 150.0f, &window);


		//sorting for quad tree building before vel calcs
		arrayMap.sort();
		//Build Quad Tree
		tree.buildNodes(arrayMap.getArrayMap());
		//get new velocities and modify placement 
	



		for (size_t i = 0; i < P_COUNT; i++)
		{
			//particles[i].setVelocity(particles[i].getVelocity() + DELTA * accelerationSum(particles, i));
		}

		//particle3.setVelocity(particle3.getVelocity() + DELTA * (
		//	acceleration(particle3.getPosition(), particle1.getPosition()) +
		//	acceleration(particle3.getPosition(), particle2.getPosition()))


		for (size_t i = 0; i < P_COUNT; i++)
		{
			//setpPosition
			//particles[i].setpPosition(particles[i].getpPosition() + DELTA * particles[i].getVelocity());
			//set shape pos
			//particles[i].setPosition(particles[i].getpPosition());
		}


		//std::cout << particles[1].getVelocity().y <<" \n";





		//this is expensive, do using a shader
		for (size_t i = 0; i < P_COUNT; i++)
		{
			circle[i].setPosition(particles[i].position);
		}
		//draw particles
		for (size_t i = 0; i < P_COUNT; i++)
		{
			window.draw(circle[i]);
		}




		///window.draw(circle);
		//window.draw(test);
		//displays the frame

		window.display();
		window.clear();
		//clear the ArrayMap
		arrayMap.clear();
		//delete Tree;



	}

	return 0;
}

sf::Vector2f acceleration(sf::Vector2f object1, sf::Vector2f object2) {

	//TODO mass is assumed to be 1
	// pow is double not gpu friendly?
	float mass = 1.0f;


	//float r = pow(object1.x, 2) + pow(object1.y, 2);


	sf::Vector2f t;
	//t.x = object2.x - object1.x;
	//t.y = object2.y - object1.y;
	t = object2 - object1;

	//no need sqrt, since we want r^3
	float r = pow(t.x, 2) + pow(t.y, 2);
	//r = abs(r * r);

	// add mass for barnes hut
	float ax = mass *
		(object2.x - object1.x) / (r + 0.00001f);
	float ay = mass *
		(object2.y - object1.y) / (r + 0.00001f);

	//std::cout << "\n "<< (object2.y - object1.y) << "\n";


	return sf::Vector2f(ax, ay);
}

sf::Vector2f accelerationSum(Particle particles[], int index) {

	sf::Vector2f accSum;


	for (size_t i = 0; i < P_COUNT; i++)
	{
		if (i != index)
		{
			//accSum = accSum + acceleration(particles[index].getpPosition(), particles[i].getpPosition());
		}
	}

	return accSum;






}