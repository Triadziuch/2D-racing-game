#include "Pickup.h"

void Pickup::initVariables(sf::Vector2f position, sf::Texture& texture, std::string type)
{
	this->scale = 4.f;
	this->type = type;
	this->sprite.setTexture(texture);
	this->sprite.setScale({ this->scale, this->scale });
	this->sprite.setPosition(position);
}

Pickup::Pickup(sf::Vector2f position, sf::Texture& texture, std::string type)
{
	this->initVariables(position, texture, type);
}

Pickup::~Pickup()
{
}

void Pickup::update(float dt)
{
}

void Pickup::move(float value)
{
	this->sprite.move({ 0.f, value });
}

void Pickup::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
