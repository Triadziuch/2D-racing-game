#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "RotationProcessing.h"
#include "AssetManager.h"

class Car
{
private:
	// Private variables
	RotationProcessing *RP;
	sf::Sprite sprite;
	float speed;
	float scale;
	float speed_factor;
	sf::RenderWindow* window;
	bool* isMenu;
	bool using_mouse;

	// Initialization functions 
	void initVariables(sf::RenderWindow* _window, bool* _isMenu);

public:
	// Constructors / Destructors
	Car(sf::RenderWindow* _window, bool* _isMenu);
	virtual ~Car();

	// Update functions
	void update(float dt);
	sf::Vector2f updateRotation();
	void updateMovement(float dt);
	void updateMovementMouse();

	// Public functions
	void move(sf::Vector2f _offset);
	void move(sf::Vector2f _offset, float _dt);
	void reset();

	// Accessors / Mutators
	sf::Vector2f getPosition() { return this->sprite.getPosition(); }
	sf::FloatRect getFloatRect() { return this->sprite.getGlobalBounds(); }
	RotationProcessing* getRP() { return this->RP; }

	void setSpeedFactor(float _speed_factor) { this->speed_factor = _speed_factor; }

	// Rendering the game
	void render(sf::RenderTarget &target);
};