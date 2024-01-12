#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Background.h"
#include "Car.h"

using namespace std;

class CollisionProcessing{
private:
	// Private variables
	Car*			 car;
	Background*		 background;
	NPCarContainer*	 NPCContainer;
	sf::FloatRect	 mapBorders;
	sf::FloatRect	 roadBorders;
	vector<Pickup*>* pickups;
	int* aktualna_lokacja, * zycia;

	// Initialization functions
	void initVariables(Car* _car, Background* _background, sf::FloatRect _mapBorders, sf::FloatRect _roadBorders, int* _aktualna_lokacja, int* _zycia);

	// Private functions
	void player_grass(float dt);
	void player_borders();
	void player_pickups();
	void player_cars();


public:
	// Constructors / Destructors
	CollisionProcessing(Car *_car, Background *_background, sf::FloatRect _mapBorders, sf::FloatRect _roadBorders, int* _aktualna_lokacja, int* _zycia);
	virtual ~CollisionProcessing();

	// Update functions
	void update(float dt);
};