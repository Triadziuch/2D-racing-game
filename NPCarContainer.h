#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <vector>
#include "NPCar.h"

class NPCarContainer
{
private:
	const int car_number = 9;
	const int max_cars = 4;
	sf::Vector2u windowSize;
	sf::Texture* texture_car;
	std::vector<NPCar*> vCars;
	float car_spawn_left, car_spawn_right;
	char last_spawn{};

	// Initialization functions 
	void initVariables(float car_spawn_left, float car_spawn_right, sf::Vector2u);
	void spawn();
	void delete_out_of_border();

public:
	// Constructors / Destructors
	NPCarContainer(float car_spawn_left, float car_spawn_right, sf::Vector2u);
	virtual ~NPCarContainer();

	// Update functions
	void update(float dt);

	// Rendering background
	void render(sf::RenderTarget& target);
};