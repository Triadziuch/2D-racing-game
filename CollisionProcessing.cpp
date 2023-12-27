#include "CollisionProcessing.h"

// Initialization functions
void CollisionProcessing::initVariables(Car* _car, Background* _background, sf::FloatRect _roadBorders)
{
	this->car = _car;
	this->background = _background;
	this->roadBorders = _roadBorders;
}

// Private functions
void CollisionProcessing::player_grass(float dt)
{
	if (!this->background->backgroundContainsV2f(this->car->getPosition())) {
		this->car->setSpeedFactor(0.6f);
		this->car->move({ 0.f, 10.f }, dt);
	}
	else
		this->car->setSpeedFactor(1.f);
}

void CollisionProcessing::player_borders()
{
	sf::FloatRect carFR = this->car->getFloatRect();
	if (carFR.left < this->roadBorders.left)
		this->car->move({ this->roadBorders.left - carFR.left, 0.f });
	else if (carFR.left + carFR.width > this->roadBorders.left + this->roadBorders.width)
		this->car->move({ (this->roadBorders.left + this->roadBorders.width) - (carFR.left + carFR.width), 0.f });
}

// Constructors / Destructors
CollisionProcessing::CollisionProcessing(Car* _car, Background* _background, sf::FloatRect _roadBorders)
{
	this->initVariables(_car, _background, _roadBorders);
}

CollisionProcessing::~CollisionProcessing()
{
}

// Update functions
void CollisionProcessing::update(float dt)
{
	this->player_grass(dt);
	this->player_borders();
}
