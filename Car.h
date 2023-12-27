#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "RotationProcessing.h"

class Car
{
private:
	RotationProcessing RP;
	sf::Texture texture;
	sf::Sprite sprite;
	float speed;
	float scale;
	float speed_factor;

	// Initialization functions 
	void initVariables(sf::Vector2u windowSize);

public:
	// Constructors / Destructors
	Car(sf::Vector2u windowSize);
	virtual ~Car();

	// Update functions
	void update(float dt);
	
	void move(sf::Vector2f _offset);
	void move(sf::Vector2f _offset, float _dt);

	// Accessors
	sf::Vector2f getPosition() { return this->sprite.getPosition(); }
	sf::FloatRect getFloatRect() { return this->sprite.getGlobalBounds(); }
	void setSpeedFactor(float _speed_factor) { this->speed_factor = _speed_factor; }

	// Rendering the game
	void render(sf::RenderTarget &target);
};