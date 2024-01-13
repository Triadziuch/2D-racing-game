#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <sstream>
#include <format>

using namespace std;

class GUI
{
private:
	// Private variables
	int* aktualna_lokacja, * zycia, * zycia_max, stare_zycia;
	string nazwy_lokacji[4];
	sf::Color kolory_nazw_lokacji[4] = { sf::Color(66, 173, 55), sf::Color(239, 182, 129), sf::Color(33, 116, 196) , sf::Color(240, 238, 236) };
	float* dystans, * mnoznik_puntkow, * punkty, * predkosc;
	bool* game_over;

	float lewa_krawedz_tekstu;
	sf::Vector2f windowSize;
	sf::FloatRect roadBorders;

	unsigned font_size;
	float padding;
	float scale;

	sf::Texture texture_serca[2];
	sf::Sprite* sprite_serca;
	sf::Font font;

	string s_mapa, s_punkty, s_mnoznik, s_dystans, s_predkosc, s_game_over;
	string s_jednostki_dystans, s_jednostki_predkosc;

	sf::Text text_mapa[2], text_punkty, text_mnoznik, text_dystans, text_predkosc, text_game_over;

	// Initialization functions 
	void initVariables(sf::Vector2f _windowSize, sf::FloatRect _roadBorders, const string(&_nazwy_lokacji)[4], int* _aktualna_lokacja, float* _punkty, float* _mnoznik_punktow, float* _dystans, float* _predkosc, int* _zycia_max, int* _zycia, bool* _game_over);

public:
	// Constructors / Destructors
	GUI(sf::Vector2f _windowSize, sf::FloatRect _roadBorders, const string(&_nazwy_lokacji)[4], int* _aktualna_lokacja, float* _punkty, float* _mnoznik_punktow, float* _dystans, float* _predkosc, int* _zycia_max, int* _zycia, bool* _game_over);
	virtual ~GUI();

	// Update functions
	void update();
	void updateText();
	void updateHearts();

	// Rendering functions
	void render(sf::RenderTarget& target);
};