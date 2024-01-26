#include "CollisionProcessing.h"

// Initialization functions
void CollisionProcessing::initVariables(Car* _car, Background* _background, sf::FloatRect _mapBorders, sf::FloatRect _roadBorders, int *_aktualna_lokacja, int *_zycia)
{
	this->car = _car;
	this->background = _background;
	this->mapBorders = _mapBorders;
	this->roadBorders = _roadBorders;
	this->aktualna_lokacja = _aktualna_lokacja;
	this->zycia = _zycia;

	this->pickups = this->background->getPickupsPtr();
	this->NPCContainer = this->background->getNPCarContainerPtr();
}

// Private functions
void CollisionProcessing::player_grass(float dt)
{
	if (!this->background->backgroundContainsV2f(this->car->getPosition())) {
		this->car->setSpeedFactor(0.6f);
		this->car->move({ 0.f, 10.f }, dt);
		this->background->setLockPoints(true);
	}
	else {
		this->car->setSpeedFactor(1.f);
		this->background->setLockPoints(false);
	}
		
}

void CollisionProcessing::player_borders()
{
	sf::FloatRect carFR = this->car->getFloatRect();

	// Left border collision
	if (carFR.left < this->mapBorders.left)
		this->car->move({ this->mapBorders.left - carFR.left, 0.f });
	// Right border collision
	else if (carFR.left + carFR.width > this->mapBorders.left + this->mapBorders.width)
		this->car->move({ (this->mapBorders.left + this->mapBorders.width) - (carFR.left + carFR.width), 0.f });

	// Top border collision
	if (carFR.top < this->mapBorders.top)
		this->car->move({ 0.f, this->mapBorders.top - carFR.top });
	// Bottom border collision
	else if (carFR.top + carFR.height > this->mapBorders.top + this->mapBorders.height)
		this->car->move({ 0.f,  (this->mapBorders.top + this->mapBorders.height) - (carFR.top + carFR.height) });

	if (*this->aktualna_lokacja == 2) {
		// Left bridge road border collision
		if (carFR.left < this->roadBorders.left - 20.f)
			this->car->move({ this->roadBorders.left - carFR.left - 20.f, 0.f });
		// Right bridge road border collision
		else if (carFR.left + carFR.width > this->roadBorders.left + this->roadBorders.width + 20.f)
			this->car->move({ (this->roadBorders.left + this->roadBorders.width + 20.f) - (carFR.left + carFR.width), 0.f });
	}
}

void CollisionProcessing::player_pickups()
{
	for (auto const& pickup : *this->pickups) {
		if (!pickup->getCollided()) {
			if (!pickup->getCurrentlyCollides()) {
				if (this->car->getFloatRect().intersects(pickup->getFloatRect())) {
					if (pickup->getType() == "dziura") {
						if (this->background->getSpeedFactor() >= 1.f)
							this->background->setSpeedFactor(1.f);

						if (this->background->getPointsFactor() >= 1.f)
							this->background->setPointsFactor(1.f);
						pickup->setCollided(true);
					}
					else if (pickup->getType() == "przyspieszenie") {
						this->background->addSpeedFactor(0.1f);
						this->background->addPointsFactor(0.25f);
						pickup->setCollided(true);
					}
					else
						pickup->setCurrentlyCollides(true);
				}
			}
			else{
				if (this->car->getFloatRect().intersects(pickup->getFloatRect())) {
					if (pickup->getType() == "pekniecie") {
						this->car->setSpeedFactor(0.25f);
					}
					if (pickup->getType() == "lod" || pickup->getType() == "smar") {
						this->car->setSpeedFactor(1.5f);
						this->car->move({ 1.f * pickup->getKierunekZjezdzania(), 0.f });
					}
				}
				else {
					if (pickup->getType() == "pekniecie") {
						this->car->setSpeedFactor(1.f);
						this->background->addSpeedFactor(-0.02f);
						this->background->addPointsFactor(-0.01f);
						pickup->setCollided(true);
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

void CollisionProcessing::player_cars()
{
	if (this->NPCContainer->intersects(this->car->getFloatRect())) 
		--*this->zycia;
}

// Constructors / Destructors
CollisionProcessing::CollisionProcessing(Car* _car, Background* _background, sf::FloatRect _mapBorders, sf::FloatRect _roadBorders, int* _aktualna_lokacja, int* _zycia)
{
	this->initVariables(_car, _background, _mapBorders, _roadBorders, _aktualna_lokacja, _zycia);
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
	this->player_cars();
}