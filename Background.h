#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string.h>
#include <vector>
#include "Droga.h"
#include "Pickup.h"

class Background
{
private:
	sf::Vector2u windowSize;
	float start_dekoracje;
	float scale;
	int aktualna_lokacja;
	const sf::Vector2i rozmiar_droga{ 12, 4 }, rozmiar_tankowanie{ 12, 19 };

	bool dekoracje_droga[4][12] = {
									    {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
									    {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
									    {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
									    {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1}
									    };
	bool dekoracje_tankowanie[19][12] = {
											  {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
											  {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1}
	};
	bool** rozmieszenie_dekoracji_droga;
	bool** rozmieszenie_dekoracji_tankowanie;

	static constexpr int liczba_lokacji = 1;
	static constexpr int liczba_typow_dekoracji = 4;
	static constexpr int liczba_typow_drog = 2;
	const float mnoznik_predkosci = 1.f;

	sf::Texture texture_dekoracje[liczba_lokacji][liczba_typow_dekoracji];
	sf::Texture** texture_dekoracje_wsk;

	sf::Texture texture_droga[liczba_lokacji][liczba_typow_drog];
	sf::Texture** texture_droga_wsk;

	std::vector <Droga*> vdroga;

	static constexpr int liczba_pickups = 5;
	sf::Texture *texture_pickups;
	int szansa_dziura, szansa_lod, szansa_pekniecie, szansa_przyspieszenie, szansa_smar;
	int max_dziura, max_lod, max_pekniecie, max_przyspieszenie, max_smar;
	int liczba_dziura{}, liczba_lod{}, liczba_pekniecie{}, liczba_przyspieszenie{}, liczba_smar{};
	std::vector <Pickup*> vpickups;

	// Initialization functions 
	void initVariables(sf::Vector2u windowSize);
	float getNajwyzszaDroga();
	void dodajDroge(float wysokosc);
	void spawnPickups();
	bool collidePickups(sf::FloatRect obj);
	void deletePickups();

public:
	// Constructors / Destructors
	Background(sf::Vector2u windowSize);
	virtual ~Background();

	void move(float value);
	bool backgroundContainsV2f(const sf::Vector2f&obj);
	sf::FloatRect getBorders();
	float getCarSpawnRight();
	float getCarSpawnLeft();

	// Update functions
	void update(float dt, float movement_offset);
	void updateDroga();
	void updatePickups();

	// Rendering background
	void render(sf::RenderTarget& target);
};