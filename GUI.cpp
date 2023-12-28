#include "GUI.h"

// Initialization functions 
void GUI::initVariables(sf::Vector2f _windowSize, sf::FloatRect _roadBorders, string* _nazwy_lokacji, int* _aktualna_lokacja, float* _punkty, float* _mnoznik_punktow, float* _dystans, float* _predkosc, int* _zycia_max, int* _zycia)
{
	this->windowSize = _windowSize;
	this->roadBorders = _roadBorders;
	this->aktualna_lokacja = _aktualna_lokacja;
	*this->nazwy_lokacji = _nazwy_lokacji;
	this->punkty = _punkty;
	this->mnoznik_puntkow = _mnoznik_punktow;
	this->dystans = _dystans;
	this->predkosc = _predkosc;
	this->zycia_max = _zycia_max;
	this->zycia = _zycia;

	this->font_size = 32u;
	this->padding = 10.f;

	if (!this->texture_serca[0].loadFromFile("Textures/Serce/Serce-pelne.png"))
		printf("ERROR: Nie udalo sie wczytac pliku Textures/Serce/Serce-pelne.png");
	if (!this->texture_serca[1].loadFromFile("Textures/Serce/Serce-puste.png"))
		printf("ERROR: Nie udalo sie wczytac pliku Textures/Serce/Serce-puste.png");

	if (!this->font.loadFromFile("Fonts/Pixel.ttf"))
		printf("ERROR: Nie udalo sie wczytac pliku Fonts/Pixel.png");

	this->s_mapa = "Mapa: ";
	this->s_punkty = "Punkty: ";
	this->s_mnoznik = "Mnoznik: x";
	this->s_dystans = "Dystans: ";
	this->s_predkosc = "Predkosc: ";
	
	this->s_jednostki_dystans = "km";
	this->s_jednostki_predkosc = "km/h";

	// Inicjalizacja tekstu "Mapa: "
	this->text_mapa[0].setFont(this->font);
	this->text_mapa[0].setCharacterSize(this->font_size);
	this->text_mapa[0].setFillColor(sf::Color::White);
	this->text_mapa[0].setString(this->s_mapa);
	this->text_mapa[0].setPosition({ this->roadBorders.left + this->roadBorders.width + this->padding, this->roadBorders.top + this->padding });

	this->text_mapa[1].setFont(this->font);
	this->text_mapa[1].setCharacterSize(this->font_size);
	this->text_mapa[1].setFillColor(this->kolory_nazw_lokacji[*this->aktualna_lokacja]);
	this->text_mapa[1].setString(*this->nazwy_lokacji[*this->aktualna_lokacja]);
	this->text_mapa[1].setPosition({ this->text_mapa[0].getGlobalBounds().left + this->text_mapa[0].getGlobalBounds().width, this->text_mapa[0].getGlobalBounds().top });
}

// Constructors / Destructors
GUI::GUI(sf::Vector2f _windowSize, sf::FloatRect _roadBorders, string* _nazwy_lokacji, int* _aktualna_lokacja, float* _punkty, float* _mnoznik_punktow, float* _dystans, float* _predkosc, int* _zycia_max, int* _zycia)
{
	this->initVariables(_windowSize, _roadBorders, _nazwy_lokacji, _aktualna_lokacja, _punkty, _mnoznik_punktow, _dystans, _predkosc, _zycia_max, _zycia);
}

GUI::~GUI()
{
}

void GUI::update()
{
}

void GUI::render(sf::RenderTarget& target)
{
	target.draw(this->text_mapa[0]);
	target.draw(this->text_mapa[1]);
}