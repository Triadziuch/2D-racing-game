#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Background.h";
#include "Car.h"
#include "NPCarContainer.h"

using namespace std;

class CollisionProcessing{
private:
	Car* car;
	Background* background;
	sf::FloatRect roadBorders;

	void player_grass(float dt);
	void player_borders();

public:
	// Constructors / Destructors
	CollisionProcessing(Car *car, Background *background, sf::FloatRect roadBorders);
	virtual ~CollisionProcessing();

	// Update functions
	void update(float dt);
};