#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <time.h>

class Pickup
{
private:
	// Private variables
	float scale;
	sf::Sprite sprite;
	std::string type;

	// Initialization functions 
	void initVariables(sf::Vector2f position, sf::Texture& texture, std::string type);

	// Private functions
	void move(float value);

public:
	// Constructors / Destructors
	Pickup(sf::Vector2f position, sf::Texture &texture, std::string type);
	virtual ~Pickup();

	// Update functions
	void update(float dt, float movement_offset);

	// Accessors / Mutators
	sf::FloatRect getFloatRect() { return this->sprite.getGlobalBounds(); }
	std::string getType() { return this->type; }

	// Rendering functions
	void render(sf::RenderTarget& target);
};