#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Particle.h"
#include <iostream>


// most performant way to render, use shader to modify texture?
// color using Viscosity ??


//temp nbody equation
sf::Vector2f acceleration(sf::Vector2f object1, sf::Vector2f object2);
//float G_CONSTANT = 6.67300 * pow(10, -11);
float G_CONSTANT = 0.0001f;

int main()
{
	int VERTICAL_RESOLUTION = 1080;
	int HORIZONTAL_RESOLUTION = 1920;
	float ASPECT_RATIO = 0;
	float DELTA = 5.0f / 1000.0f;// 
	//float DELTA = 1.0f;



	sf::RenderWindow window(sf::VideoMode(HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION), "N-Body Simulation");
	window.setFramerateLimit(144);

	sf::View camera(sf::Vector2f(0.f, 0.f), sf::Vector2f(300.f, 200.f));

	sf::CircleShape shape1(1.0f);
	sf::CircleShape shape2(1.0f);
	sf::CircleShape shape3(1.0f);

	Particle particle1(sf::Vector2f(0, 1));
	Particle particle2(sf::Vector2f(-5, 0));
	Particle particle3(sf::Vector2f(5, 0));

	particle1.setVelocity(sf::Vector2f(0,0));
	particle2.setVelocity(sf::Vector2f(-1,1));
	particle3.setVelocity(sf::Vector2f(1,-1));


	shape1.setFillColor(sf::Color(250, 10, 10));
	shape2.setFillColor(sf::Color(10, 250, 10));
	shape3.setFillColor(sf::Color(10, 10, 250));

	//shape1.setPosition(sf::Vector2f(20, 60));
	//shape1.setPosition(sf::Vector2f(40, 70));
	//shape1.setPosition(sf::Vector2f(60, 80));


	// run the program as long as the window is open
	bool moveUp = false;
	bool moveDown = false;
	bool moveLeft = false;
	bool moveRight = false;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				camera.move(0.f, -5.f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				camera.move(0.f, 5.f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				camera.move(-5.f, 0.f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				camera.move(5.f, 0.f);

			//std::cout << sf::Event::MouseWheelScrollEvent::delta;


			//event.mouseWheelScroll.delta
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
				camera.zoom(0.95f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
				camera.zoom(1.05f);


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();

			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}


		//particle1.setVelocity(particle1.getVelocity() + DELTA *
		//	acceleration(particle1.getPosition(), particle2.getPosition()));
		particle1.setVelocity(particle1.getVelocity() + DELTA* (
			acceleration(particle1.getPosition(), particle2.getPosition()) +
			acceleration(particle1.getPosition(), particle3.getPosition()))
		);

		particle2.setVelocity(particle2.getVelocity() + DELTA * (
			acceleration(particle2.getPosition(), particle1.getPosition()) +
			acceleration(particle2.getPosition(), particle3.getPosition()))
		);

		particle3.setVelocity(particle3.getVelocity() + DELTA * (
			acceleration(particle3.getPosition(), particle1.getPosition()) +
			acceleration(particle3.getPosition(), particle2.getPosition()))
		);







		particle1.setPosition(particle1.getPosition() + DELTA * particle1.getVelocity());
		particle2.setPosition(particle2.getPosition() + DELTA * particle2.getVelocity());
		particle3.setPosition(particle3.getPosition() + DELTA * particle3.getVelocity());

		shape1.setPosition(particle1.getPosition());
		shape2.setPosition(particle2.getPosition());
		shape3.setPosition(particle3.getPosition());


		//shape1.move(sf::Vector2f(10, 10));

		std::cout << "Velocity: " << particle1.getVelocity().y << " ";
		std::cout << "Position: " << particle1.getPosition().y << " ";
		std::cout << "Acceleration: " << acceleration(particle1.getPosition(), particle2.getPosition()).y << " ";
		
		std::cout << std::endl;







		window.setView(camera);


		//draw particles
		window.draw(shape1);
		window.draw(shape2);
		window.draw(shape3);



		//displays the frame

		window.display();
		window.clear();



	}

	return 0;
}

sf::Vector2f acceleration(sf::Vector2f object1, sf::Vector2f object2) {

	//TODO mass is assumed to be 1
	// pow is double not gpu friendly?
	float mass = 1.0f;


	float r = sqrt(pow(object2.x, 2) + pow(object2.y, 2));
	
	float ax = mass *
		(object2.x - object1.x)  / r*r + 0.0001f;
	float ay = mass *
		(object2.y - object1.y)  / r*r + 0.0001f;

	//std::cout << "\n "<< (object2.y - object1.y) << "\n";


	return sf::Vector2f(ax, ay);
}