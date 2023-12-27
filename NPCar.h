#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <time.h>

class NPCar
{
private:
	sf::Sprite sprite;
	float speed;
	int direction;
	float scale;

	// Initialization functions 
	void initVariables(sf::Texture& texture, float speed, sf::Vector2f spawn_pos, int direction);

public:
	// Constructors / Destructors
	NPCar(sf::Texture& texture, float speed, sf::Vector2f spawn_pos, int direction);
	virtual ~NPCar();

	// Update functions
	void update(float dt);

	sf::FloatRect getFloatRect() { return this->sprite.getGlobalBounds(); }

	// Rendering background
	void render(sf::RenderTarget& target);
};