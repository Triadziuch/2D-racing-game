#include "CollisionProcessing.h"

// Initialization functions
void CollisionProcessing::initVariables(Car* _car, Background* _background, sf::FloatRect _roadBorders)
{
	this->car = _car;
	this->background = _background;
	this->roadBorders = _roadBorders;
	
	this->pickups = this->background->getPickupsPtr();
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

	// Left border collision
	if (carFR.left < this->roadBorders.left)
		this->car->move({ this->roadBorders.left - carFR.left, 0.f });
	// Right border collision
	else if (carFR.left + carFR.width > this->roadBorders.left + this->roadBorders.width)
		this->car->move({ (this->roadBorders.left + this->roadBorders.width) - (carFR.left + carFR.width), 0.f });

	// Top border collision
	if (carFR.top < this->roadBorders.top)
		this->car->move({ 0.f, this->roadBorders.top - carFR.top });
	// Bottom border collision
	else if (carFR.top + carFR.height > this->roadBorders.top + this->roadBorders.height)
		this->car->move({ 0.f,  (this->roadBorders.top + this->roadBorders.height) - (carFR.top + carFR.height) });
}

void CollisionProcessing::player_pickups()
{
	for (auto const& pickup : *this->pickups) {
		if (!pickup->getCollided()) { //dziura przyspieszenie
			if (!pickup->getCurrentlyCollides()) { //pierwsza kolizja
				if (this->car->getFloatRect().intersects(pickup->getFloatRect())) {
					if (pickup->getType() == "dziura") {
						printf("KOLIZJA: DZIURA\n");
						pickup->setCollided(true);
					}
					else if (pickup->getType() == "przyspieszenie") {
						printf("KOLIZJA: PRZYSPIESZENIE\n");
						this->background->addSpeedFactor(0.05f);
						pickup->setCollided(true);
					}
					else
						pickup->setCurrentlyCollides(true);
				}
			}
			if (pickup->getCurrentlyCollides()) {
				if (this->car->getFloatRect().intersects(pickup->getFloatRect())) { // co robimy kiedy jesteœmy na obiekcie
					if (pickup->getType() == "pekniecie") {
						this->car->setSpeedFactor(0.25f);
					}
					else if (pickup->getType() == "lod" || pickup->getType() == "smar") {
						this->car->setSpeedFactor(1.5f);
						this->car->move({ 1.f * pickup->getKierunekZjezdzania(), 0.f });
					}
				}
				else { // co robimy kiedy zjedziemy z obiektu
					if (pickup->getType() == "pekniecie") {
						this->car->setSpeedFactor(1.f);
					}
					else if (pickup->getType() == "lod" || pickup->getType() == "smar") {
						this->car->setSpeedFactor(1.f);
					}
					pickup->setCurrentlyCollides(false);
				}
			}
			
		}
		else {

		}
	}
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
	this->player_pickups();
}