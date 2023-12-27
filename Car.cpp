#include "Car.h"

void Car::initVariables(sf::Vector2u windowSize)
{
	this->speed = 30.f;
	this->scale = 4.f;
	this->texture.loadFromFile("Textures/Samochody/car4.png");
	this->sprite.setTexture(this->texture);
	this->sprite.setOrigin(this->sprite.getGlobalBounds().left + this->sprite.getGlobalBounds().width / 2.f, this->sprite.getGlobalBounds().top + this->sprite.getGlobalBounds().height / 2.f);
	this->sprite.setScale({ this->scale, this->scale });
	this->sprite.setPosition(static_cast<sf::Vector2f>(windowSize / 2u));
	this->speed_factor = 1.f;
}

Car::Car(sf::Vector2u windowSize)
{
	this->initVariables(windowSize);
}

Car::~Car()
{
}

void Car::update(float dt)
{
	this->RP.update();

	float rotation_x = static_cast<float>(this->RP.getNewLR()), rotation_y = static_cast<float>(this->RP.getNewFB());
	
	float kierunek = signbit(rotation_x) ? -1.f : 1.f;
	if (abs(rotation_x) < 2.f) {
		rotation_x = 0.f;
		this->sprite.setRotation(0.f);
	}
	else if (abs(rotation_x) < 8.f) {
		this->sprite.setRotation(8.f * kierunek);
	}
	else if (abs(rotation_x) < 16.f)
		this->sprite.setRotation(16.f * kierunek);
	else
		this->sprite.setRotation(24.f * kierunek);
		
	if (abs(rotation_y) < 1.f)
		rotation_y = 0.f;

	
	this->sprite.move({ rotation_x * dt * this->speed * this->speed_factor, rotation_y * dt * this->speed * this->speed_factor });
}

void Car::render(sf::RenderTarget &target)
{
	target.draw(this->sprite);
}

void Car::move(sf::Vector2f _offset)
{
	this->sprite.move(_offset);
}

void Car::move(sf::Vector2f _offset, float _dt)
{
	this->sprite.move(_offset * _dt * this->speed * this->speed_factor);
}