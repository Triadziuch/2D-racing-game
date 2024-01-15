#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include "AssetManager.h"

using namespace std;

class Leaderboard 
{
private:
	// Private variables
	float *points;
	sf::Font font;
	unsigned font_size;
	float column_width;
	string filename;
	sf::RenderWindow* window;
	string nazwa_string{};
	bool* isEnd;
	bool enteringName;

	static constexpr int wpisow = 10;
	string names[wpisow]{};
	string leaderboard_points[wpisow]{};

	sf::Text text_leaderboard[wpisow + 1][3];
	string s_nazwy_kolumn[3] = {"POZYCJA", "NAZWA", "PUNKTY"};

	sf::Text text_nazwa, text_podaj_nazwe;
	string s_podaj_nazwe;

	sf::Text text_przejdz_do_menu;
	string s_przejdz_do_menu;

	// Initialization functions 
	void initVariables(sf::RenderWindow* _window, float* _points, bool* _isEnd);

	// Private functions
	void addNameToLeaderboard();

public:
	// Constructors / Destructors
	Leaderboard(sf::RenderWindow* _window, float* _points, bool* _isEnd);
	virtual ~Leaderboard();

	// Update functions
	void update();
	void updatePollEvents();

	// Public functions
	void save();

	// Rendering functions
	void render(sf::RenderTarget& target);
};