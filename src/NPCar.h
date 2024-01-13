#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <time.h>

class NPCar
{
private:
	// Private variables
	sf::Sprite	 sprite;
	float		 speed;
	int			 direction;
	float		 scale;
	float*		 speed_multiplier;

	// Initialization functions 
	void initVariables(sf::Texture& _texture, float _speed, sf::Vector2f _spawn_pos, int _direction, float *_speed_multiplier);

	// Private functions
	void move(float dt);

public:
	// Constructors / Destructors
	NPCar(sf::Texture& _texture, float _speed, sf::Vector2f _spawn_pos, int _direction, float* _speed_multiplier);
	virtual ~NPCar();

	// Update functions
	void update(float dt);

	// Accessors / Mutators
	sf::FloatRect getFloatRect() { return this->sprite.getGlobalBounds(); }

	// Rendering functions
	void render(sf::RenderTarget& target);
};