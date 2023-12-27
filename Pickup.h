#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <time.h>

class Pickup
{
private:
	// Initialization functions 
	float scale;
	sf::Sprite sprite;
	std::string type;

	void initVariables(sf::Vector2f position, sf::Texture& texture, std::string type);

public:
	// Constructors / Destructors
	Pickup(sf::Vector2f position, sf::Texture &texture, std::string type);
	virtual ~Pickup();

	// Update functions
	void update(float dt);

	sf::FloatRect getFloatRect() { return this->sprite.getGlobalBounds(); }
	std::string getType() { return this->type; }

	void move(float value);

	void render(sf::RenderTarget& target);
};