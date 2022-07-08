#pragma once
#include <SFML/Graphics.hpp>
#include "GlobalVariables.h"

struct Particle
{
	sf::Vector2f position;
	sf::Vector2f vel;
	sf::Vector2f acceleration;
	int mass = 1;//default 1 mass
};
