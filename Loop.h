#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#include <map>
#include <time.h>
#include <string.h>
#include <cstring>
#include <iomanip>
#include <locale>
#include <Windows.h>
#include <fstream>
#include "CollisionProcessing.h"
#include "GUI.h"

class Loop
{
private:
	// Window variables
	sf::RenderWindow* window;

	// is variables
	bool isEnd;

	// Clock variables
	sf::Clock dt_clock;

	// Object variables
	Background* background;
	Car *car;
	CollisionProcessing *collisionProcessing;
	GUI* gui;

	float dt;
	float background_moving_speed;

	int aktualna_lokacja, zycia_max, zycia;
	string nazwy_lokacji[4] = { "Podlasie", "Pustynia", "Most", "Arktyka" };
	float dystans, mnoznik_predkosci, mnoznik_puntkow, punkty, predkosc;

	// Background color
	sf::Color color_background;

	// Initialization functions 
	void initWindow();
	void initVariables();

public:
	// Constructors / Destructors
	Loop();
	virtual ~Loop();

	// Run functions
	void run();
	
	// Update functions
	void update();
	void updatePollEvents();

	// Rendering functions
	void render();
};