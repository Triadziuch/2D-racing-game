#include "Pickup.h"

// Initialization functions 
void Pickup::initVariables(sf::Vector2f position, sf::Texture& texture, std::string type)
{
	this->scale = 4.f;
	this->type = type;
	this->sprite.setTexture(texture);
	this->sprite.setScale({ this->scale, this->scale });
	this->sprite.setPosition(position);
}

// Private functions
void Pickup::move(float value)
{
	this->sprite.move({ 0.f, value });
}

// Constructors / Destructors
Pickup::Pickup(sf::Vector2f position, sf::Texture& texture, std::string type)
{
	this->initVariables(position, texture, type);
}

Pickup::~Pickup()
{
}

// Update functions
void Pickup::update(float dt, float movement_offset)
{
	this->move(dt * movement_offset);
}

// Rendering functions
void Pickup::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
