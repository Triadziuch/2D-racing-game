#pragma once
#include <string>
#include "NPCar.h"
#include "Animator.h"

class NPCarContainer
{
private:
	// Private variables
	const int				 car_number = 9;
	const int				 max_cars = 4;
	sf::Vector2u			 windowSize;
	std::vector<NPCar*>		 vCars;
	float					 car_spawn_left, car_spawn_right;
	char					 last_spawn{};
	float*					 speed_multiplier;
	bool*					 started_transition;

	// Animation variables
	std::vector<sf::Sprite*> vExplosionSprite;
	std::vector<Animator*>   vExplosionAnimator;
	std::string				 explosion_animation_filename;

	// Initialization functions 
	void initVariables(float _car_spawn_left, float _car_spawn_right, sf::Vector2u _windowSize, float* _speed_multiplier, bool* _started_transition);

	// Private functions
	void spawn();
	void BOOM(NPCar& car);
	void delete_out_of_border();

public:
	// Constructors / Destructors
	NPCarContainer(float _car_spawn_left, float _car_spawn_right, sf::Vector2u _windowSize, float* _speed_multiplier, bool* _started_transition);
	virtual ~NPCarContainer();

	// Update functions
	void update(float dt, float movement_offset);
	void updateNPCars(float dt);
	void updateAnimators(float dt, float movement_offset);

	// Public functions
	bool intersects(const sf::FloatRect &rectangle);

	// Rendering functions
	void render(sf::RenderTarget& target);
};