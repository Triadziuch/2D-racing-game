#include "NPCar.h"

// Initialization functions 
void NPCar::initVariables(sf::Texture &_texture, float _speed, sf::Vector2f _spawn_pos, int _direction)
{
	this->speed = _speed;
	this->direction = _direction;
	this->scale = 4.f;
	this->sprite.setTexture(_texture);
	this->sprite.setScale({ this->scale, this->scale });
	this->sprite.setPosition(_spawn_pos);

	if (this->direction == 1) {
		this->sprite.setRotation(180.f);
		this->sprite.move({ this->sprite.getGlobalBounds().width, 0.f });
	}
	else
		this->sprite.move({ -this->sprite.getGlobalBounds().width, 0.f });
}

// Private functions
void NPCar::move(float dt)
{
	this->sprite.move({ 0.f, this->direction * this->speed * dt });
}

// Constructors / Destructors
NPCar::NPCar(sf::Texture& _texture, float _speed, sf::Vector2f _spawn_pos, int _direction)
{
	this->initVariables(_texture, _speed, _spawn_pos, _direction);
}

NPCar::~NPCar()
{
}

// Update functions
void NPCar::update(float dt)
{
	this->move(dt);
}

// Rendering functions
void NPCar::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
