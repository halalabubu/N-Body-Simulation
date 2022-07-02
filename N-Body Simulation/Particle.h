#pragma once

#include <SFML/Graphics.hpp>
class Particle
{
public:
	//random direction and magnitude if given no param
	Particle();
	Particle(sf::Vector2f position);
	Particle(sf::Vector2f position, sf::Vector2f vel, float magnitude, float mass);
	~Particle();

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f position);
	sf::Vector2f getVelocity();
	void setVelocity(sf::Vector2f vel);

private:
	sf::Vector2f position;
	sf::Vector2f vel;
	sf::Vector2f acceleration;
	float magnitude;
	float mass;

};

Particle::Particle()
{
	magnitude = 0;
	mass = 1;
}

inline Particle::Particle(sf::Vector2f position)
{
	this->position = position;
}

inline Particle::Particle(sf::Vector2f position, sf::Vector2f vel, float magnitude, float mass)
{
	this->position = position;
	this->vel = vel;
	this->magnitude = magnitude;
	this->mass = mass;
	acceleration.x = 0;
	acceleration.y = 0;
}

Particle::~Particle()
{
}

inline sf::Vector2f Particle::getPosition()
{
	return position;
}

inline void Particle::setPosition(sf::Vector2f position)
{
	this->position = position;
}

inline sf::Vector2f Particle::getVelocity()
{
	return vel;
}

inline void Particle::setVelocity(sf::Vector2f vel)
{
	this->vel = vel;
}
