#pragma once
#include <string>
#include <vector>
#include "NPCar.h"

class NPCarContainer
{
private:
	// Private variables
	const int			 car_number = 9;
	const int			 max_cars = 4;
	sf::Vector2u		 windowSize;
	sf::Texture*		 texture_car;
	std::vector<NPCar*>	 vCars;
	float				 car_spawn_left, car_spawn_right;
	char				 last_spawn{};

	// Initialization functions 
	void initVariables(float _car_spawn_left, float _car_spawn_right, sf::Vector2u _windowSize);

	// Private functions
	void spawn();
	void delete_out_of_border();

public:
	// Constructors / Destructors
	NPCarContainer(float _car_spawn_left, float _car_spawn_right, sf::Vector2u _windowSize);
	virtual ~NPCarContainer();

	// Update functions
	void update(float dt);

	// Rendering functions
	void render(sf::RenderTarget& target);
};