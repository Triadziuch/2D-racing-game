#include "Pickup.h"

// Initialization functions 
void Pickup::initVariables(sf::Vector2f _position, sf::Texture& _texture, std::string _type, float _center)
{
	this->scale = 4.f;
	this->type = _type;
	this->collided = false;
	this->currently_collides = false;
	if (rand() % 2 == 0)
		this->kierunek_zjezdzania = 1;
	else
		this->kierunek_zjezdzania = -1;

	this->sprite.setTexture(_texture);
	this->sprite.setOrigin({ this->sprite.getGlobalBounds().left - this->sprite.getGlobalBounds().width / 2.f, this->sprite.getGlobalBounds().height - this->sprite.getGlobalBounds().height / 2.f});
	this->sprite.setScale({ this->scale, this->scale });
	this->sprite.setPosition(_position);

	// Make sure the pickup is not in the center of the road
	float sprite_left = this->sprite.getGlobalBounds().left, sprite_right = this->sprite.getGlobalBounds().left + this->sprite.getGlobalBounds().width;
	
	if (sprite_left < _center && sprite_right > _center) {
		// Move to the right from the center
		if (_center - sprite_left <= sprite_right - _center) 
			this->sprite.move({ _center - sprite_left, 0.f });

		// Move to the left from the center
		else 
			this->sprite.move({ _center - sprite_right, 0.f });
	}
}

// Private functions
void Pickup::move(sf::Vector2f value)
{
	this->sprite.move(value);
}

// Constructors / Destructors
Pickup::Pickup(sf::Vector2f _position, sf::Texture& _texture, std::string _type, float _center)
{
	this->initVariables(_position, _texture, _type, _center);
}

Pickup::~Pickup()
{
}

// Update functions
void Pickup::update(float dt, float movement_offset)
{
	this->move({ 0.f, dt * movement_offset });
}

// Rendering functions
void Pickup::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
