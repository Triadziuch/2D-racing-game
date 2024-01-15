#pragma once
#include <vector>
#include <iostream>
#include <map>
#include <time.h>
#include <string.h>
#include <cstring>
#include <iomanip>
#include <locale>
#include <fstream>
#include "CollisionProcessing.h"
#include "GUI.h"
#include "Leaderboard.h"
#include "Menu.h"

using namespace std;

class Loop
{
private:
	// Window variables
	sf::RenderWindow* window;

	// is variables
	bool isEnd, isMenu;

	// Clock variables
	sf::Clock dt_clock;

	// Object variables
	Background* background;
	Car *car;
	CollisionProcessing *collisionProcessing;
	GUI* gui;
	Leaderboard *leaderboard;
	Menu* menu;
	AssetManager manager;

	string test_string{};
	sf::Font font;
	sf::Text text_test;

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
	void initAssets();
	void initNewGame();

public:
	// Constructors / Destructors
	Loop();
	virtual ~Loop();

	// Run functions
	void run();
	
	// Update functions
	void update();
	void updateMenu();
	void updateGame();
	void updateLeaderboard();
	void updateGameOver();
	void updatePollEvents();

	// Rendering functions
	void render();
};