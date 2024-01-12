#include "GUI.h"

// Initialization functions 
void GUI::initVariables(sf::Vector2f _windowSize, sf::FloatRect _roadBorders, const string(&_nazwy_lokacji)[4], int* _aktualna_lokacja, float* _punkty, float* _mnoznik_punktow, float* _dystans, float* _predkosc, int* _zycia_max, int* _zycia, bool* _game_over)
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
	this->game_over = _game_over;
	this->stare_zycia = *this->zycia;

	this->font_size = 28u;
	this->padding = 20.f;
	this->scale = 3.f;

	if (!this->texture_serca[0].loadFromFile("Textures/Serce/Serce-puste.png"))
		printf("ERROR: Nie udalo sie wczytac pliku Textures/Serce/Serce-puste.png");
	if (!this->texture_serca[1].loadFromFile("Textures/Serce/Serce-pelne.png"))
		printf("ERROR: Nie udalo sie wczytac pliku Textures/Serce/Serce-pelne.png");

	this->sprite_serca = new sf::Sprite[*this->zycia_max];

	if (!this->font.loadFromFile("Fonts/Pixel.ttf"))
		printf("ERROR: Nie udalo sie wczytac pliku Fonts/Pixel.png");

	this->s_mapa = "Mapa: ";
	this->s_punkty = "Punkty: ";
	this->s_mnoznik = "Mnoznik: x";
	this->s_dystans = "Dystans: ";
	this->s_predkosc = "Predkosc: ";
	this->s_game_over = "GAME OVER";
	
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

	// Inicjalizacja tekstu Game Over
	this->text_game_over.setFont(this->font);
	this->text_game_over.setCharacterSize(48u);
	this->text_game_over.setFillColor(sf::Color::White);
	this->text_game_over.setOutlineThickness(4u);
	this->text_game_over.setOutlineColor(sf::Color::Black);
	this->text_game_over.setString(this->s_game_over);
	this->text_game_over.setPosition({ (this->windowSize.x - this->text_game_over.getGlobalBounds().width) / 2.f, (this->windowSize.y - this->text_game_over.getGlobalBounds().height) / 2.f });
	
	// Inicjalizacja sprite'ów serc
	float szerokosc_serc = this->texture_serca[0].getSize().x * this->scale * *this->zycia_max + (*this->zycia_max - 1) * 16.f;
	float poczatek_serc = (this->windowSize.x - this->roadBorders.left - this->roadBorders.width - szerokosc_serc) / 2.f + this->roadBorders.left + this->roadBorders.width;

	for (int i = 0; i < *this->zycia_max; ++i) {
		this->sprite_serca[i].setTexture(this->texture_serca[(i < *this->zycia)]);
		this->sprite_serca[i].setScale({ this->scale, this->scale });
		this->sprite_serca[i].setPosition({ poczatek_serc + this->sprite_serca[i].getGlobalBounds().width * i + 16.f * i, this->text_predkosc.getGlobalBounds().top + this->text_predkosc.getGlobalBounds().height * 3.f });
	}
}

// Constructors / Destructors
GUI::GUI(sf::Vector2f _windowSize, sf::FloatRect _roadBorders, const string(&_nazwy_lokacji)[4], int* _aktualna_lokacja, float* _punkty, float* _mnoznik_punktow, float* _dystans, float* _predkosc, int* _zycia_max, int* _zycia, bool* _game_over)
{
	this->initVariables(_windowSize, _roadBorders, _nazwy_lokacji, _aktualna_lokacja, _punkty, _mnoznik_punktow, _dystans, _predkosc, _zycia_max, _zycia, _game_over);
}

GUI::~GUI()
{
}

void GUI::update()
{
	this->updateText();
	this->updateHearts();
}

void GUI::updateText()
{
	this->text_mapa[1].setString(this->nazwy_lokacji[*this->aktualna_lokacja]);
	this->text_mapa[1].setFillColor(this->kolory_nazw_lokacji[*this->aktualna_lokacja]);
	this->text_punkty.setString(this->s_punkty + to_string(static_cast<int>(*this->punkty)));
	this->text_mnoznik.setString(this->s_mnoznik + format("{:.2f}", *this->mnoznik_puntkow));
	this->text_dystans.setString(this->s_dystans + to_string(static_cast<int>(*this->dystans)) + this->s_jednostki_dystans);
	this->text_predkosc.setString(this->s_predkosc + to_string(static_cast<int>(*this->predkosc)) + this->s_jednostki_predkosc);
}

void GUI::updateHearts()
{
	if (this->stare_zycia != *this->zycia) {
		for (int i = 0; i < *this->zycia_max; ++i)
			this->sprite_serca[i].setTexture(this->texture_serca[(i < *this->zycia)]);
		this->stare_zycia = *this->zycia;
	}
}


void GUI::render(sf::RenderTarget& target)
{
	target.draw(this->text_mapa[0]);
	target.draw(this->text_mapa[1]);
	target.draw(this->text_punkty);
	target.draw(this->text_mnoznik);
	target.draw(this->text_dystans);
	target.draw(this->text_predkosc);

	for (int i = 0; i < *this->zycia_max; ++i)
		target.draw(this->sprite_serca[i]);

	if (*this->game_over)
		target.draw(this->text_game_over);
}