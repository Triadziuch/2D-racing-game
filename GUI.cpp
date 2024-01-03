#include "GUI.h"

// Initialization functions 
void GUI::initVariables(sf::Vector2f _windowSize, sf::FloatRect _roadBorders, const string(&_nazwy_lokacji)[4], int* _aktualna_lokacja, float* _punkty, float* _mnoznik_punktow, float* _dystans, float* _predkosc, int* _zycia_max, int* _zycia)
{
	this->windowSize = _windowSize;
	this->roadBorders = _roadBorders;
	this->aktualna_lokacja = _aktualna_lokacja;

	for (int i = 0; i < 4; ++i)
		this->nazwy_lokacji[i] = _nazwy_lokacji[i];

	this->punkty = _punkty;
	this->mnoznik_puntkow = _mnoznik_punktow;
	this->dystans = _dystans;
	this->predkosc = _predkosc;
	this->zycia_max = _zycia_max;
	this->zycia = _zycia;

	this->font_size = 28u;
	this->padding = 20.f;

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

	this->lewa_krawedz_tekstu = this->roadBorders.left + this->roadBorders.width + this->padding;

	// Inicjalizacja tekstu "Mapa: "
	this->text_mapa[0].setFont(this->font);
	this->text_mapa[0].setCharacterSize(this->font_size);
	this->text_mapa[0].setFillColor(sf::Color::White);
	this->text_mapa[0].setString(this->s_mapa);
	this->text_mapa[0].setPosition({ this->lewa_krawedz_tekstu , this->roadBorders.top + this->padding });

	this->text_mapa[1].setFont(this->font);
	this->text_mapa[1].setCharacterSize(this->font_size);
	this->text_mapa[1].setFillColor(this->kolory_nazw_lokacji[*this->aktualna_lokacja]);
	this->text_mapa[1].setString(this->nazwy_lokacji[*this->aktualna_lokacja]);
	this->text_mapa[1].setPosition({ this->text_mapa[0].getGlobalBounds().left + this->text_mapa[0].getGlobalBounds().width, this->text_mapa[0].getGlobalBounds().top });

	// Inicjalizacja tekstu "Punkty: "
	this->text_punkty.setFont(this->font);
	this->text_punkty.setCharacterSize(this->font_size);
	this->text_punkty.setFillColor(sf::Color::White);
	this->text_punkty.setString(this->s_punkty + to_string(static_cast<int>(*this->punkty)));
	this->text_punkty.setPosition({ this->lewa_krawedz_tekstu, this->text_mapa[0].getGlobalBounds().top + this->text_mapa[0].getGlobalBounds().height * 2.f });

	// Inicjalizacja tekstu "Mnoznik: x"
	this->text_mnoznik.setFont(this->font);
	this->text_mnoznik.setCharacterSize(this->font_size);
	this->text_mnoznik.setFillColor(sf::Color::White);
	this->text_mnoznik.setString(this->s_mnoznik + format("{:.2f}", *this->mnoznik_puntkow));
	this->text_mnoznik.setPosition({ this->lewa_krawedz_tekstu, this->text_punkty.getGlobalBounds().top + this->text_punkty.getGlobalBounds().height * 2.f });

	// Inicjalizacja tekstu "Dystans: "
	this->text_dystans.setFont(this->font);
	this->text_dystans.setCharacterSize(this->font_size);
	this->text_dystans.setFillColor(sf::Color::White);
	this->text_dystans.setString(this->s_dystans + to_string(static_cast<int>(*this->dystans)) + this->s_jednostki_dystans);
	this->text_dystans.setPosition({ this->lewa_krawedz_tekstu, this->text_mnoznik.getGlobalBounds().top + this->text_mnoznik.getGlobalBounds().height * 2.f });

	// Inicjalizacja tekstu "Predkosc: "
	this->text_predkosc.setFont(this->font);
	this->text_predkosc.setCharacterSize(this->font_size);
	this->text_predkosc.setFillColor(sf::Color::White);
	this->text_predkosc.setString(this->s_predkosc + to_string(static_cast<int>(*this->predkosc)) + this->s_jednostki_predkosc);
	this->text_predkosc.setPosition({ this->lewa_krawedz_tekstu, this->text_dystans.getGlobalBounds().top + this->text_dystans.getGlobalBounds().height * 2.f });
}

// Constructors / Destructors
GUI::GUI(sf::Vector2f _windowSize, sf::FloatRect _roadBorders, const string(&_nazwy_lokacji)[4], int* _aktualna_lokacja, float* _punkty, float* _mnoznik_punktow, float* _dystans, float* _predkosc, int* _zycia_max, int* _zycia)
{
	this->initVariables(_windowSize, _roadBorders, _nazwy_lokacji, _aktualna_lokacja, _punkty, _mnoznik_punktow, _dystans, _predkosc, _zycia_max, _zycia);
}

GUI::~GUI()
{
}

void GUI::update()
{
	this->updateText();
}

void GUI::updateText()
{
	this->text_mapa[1].setString(this->nazwy_lokacji[*this->aktualna_lokacja]);
	this->text_punkty.setString(this->s_punkty + to_string(static_cast<int>(*this->punkty)));
	this->text_mnoznik.setString(this->s_mnoznik + format("{:.2f}", *this->mnoznik_puntkow));
	this->text_dystans.setString(this->s_dystans + to_string(static_cast<int>(*this->dystans)) + this->s_jednostki_dystans);
	this->text_predkosc.setString(this->s_predkosc + to_string(static_cast<int>(*this->predkosc)) + this->s_jednostki_predkosc);
}


void GUI::render(sf::RenderTarget& target)
{
	target.draw(this->text_mapa[0]);
	target.draw(this->text_mapa[1]);
	target.draw(this->text_punkty);
	target.draw(this->text_mnoznik);
	target.draw(this->text_dystans);
	target.draw(this->text_predkosc);
}