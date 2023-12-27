#include "NPCar.h"

void NPCar::initVariables(sf::Texture &texture, float speed, sf::Vector2f spawn_pos, int direction)
{
	this->speed = speed;
	this->direction = direction;
	this->scale = 4.f;
	this->sprite.setTexture(texture);
	this->sprite.setScale({ this->scale, this->scale });
	this->sprite.setPosition(spawn_pos);

	if (this->direction == 1) {
		this->sprite.setRotation(180.f);
		this->sprite.move({ this->sprite.getGlobalBounds().width, 0.f });
	}
	else
		this->sprite.move({ -this->sprite.getGlobalBounds().width, 0.f });
}

NPCar::NPCar(sf::Texture& texture, float speed, sf::Vector2f spawn_pos, int direction)
{
	this->initVariables(texture, speed, spawn_pos, direction);
}

NPCar::~NPCar()
{
}

void NPCar::update(float dt)
{
	this->sprite.move({ 0.f, this->direction * this->speed * dt });
}

void NPCar::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
