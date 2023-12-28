#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Background.h";
#include "Car.h"
#include <iostream>

using namespace std;

class CollisionProcessing{
private:
	// Private variables
	Car* car;
	Background* background;
	sf::FloatRect roadBorders;
	vector<Pickup*>* pickups;

	// Initialization functions
	void initVariables(Car* _car, Background* _background, sf::FloatRect _roadBorders);

	// Private functions
	void player_grass(float dt);
	void player_borders();
	void player_pickups();

public:
	// Constructors / Destructors
	CollisionProcessing(Car *_car, Background *_background, sf::FloatRect _roadBorders);
	virtual ~CollisionProcessing();

	// Update functions
	void update(float dt);
};