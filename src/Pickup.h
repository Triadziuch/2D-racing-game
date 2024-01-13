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
	bool collided;
	bool currently_collides;
	int kierunek_zjezdzania;

	// Initialization functions 
	void initVariables(sf::Vector2f _position, sf::Texture& _texture, std::string _type, float _center);

	// Private functions
	void move(sf::Vector2f value);

public:
	// Constructors / Destructors
	Pickup(sf::Vector2f _position, sf::Texture &_texture, std::string _type, float _center);
	virtual ~Pickup();

	// Update functions
	void update(float dt, float movement_offset);

	// Accessors / Mutators
	sf::FloatRect getFloatRect() { return this->sprite.getGlobalBounds(); }
	std::string getType() { return this->type; }
	bool getCollided() { return this->collided; }
	bool getCurrentlyCollides() { return this->currently_collides; }
	int getKierunekZjezdzania() { return this->kierunek_zjezdzania; }

	void setCollided(bool value) { this->collided = value; }
	void setCurrentlyCollides(bool value) { this->currently_collides = value; }

	// Rendering functions
	void render(sf::RenderTarget& target);
};