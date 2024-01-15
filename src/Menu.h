#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include "AssetManager.h"

using namespace std;

class Menu
{
private:
	// Window variables
	sf::RenderWindow* window;

	// Menu variables
	static constexpr int liczba_opcji = 4;

	int wybrana_opcja = 0;
	bool zmieniono = false;
	bool* isMenu, isTworcy;

	sf::Sprite	 logo_sprite, pb_sprite, cursor_sprite;
	sf::Font*	 font;
	unsigned	 font_size;
	sf::Text*	 text_menu;
	string*		 s_text_menu;

	sf::Text	 text_tworcy[3];

	// Initialization functions 
	void initVariables(sf::RenderWindow* _window, bool* _isMenu);

public:
	// Constructors / Destructors
	Menu(sf::RenderWindow* _window, bool* _isMenu);
	virtual ~Menu();

	// Update functions
	bool update();
	bool updatePollEvents();

	// Accessors / Mutators
	int getOpcja() { return this->wybrana_opcja; }

	// Rendering functions
	void render();
};