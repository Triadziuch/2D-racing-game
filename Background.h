#pragma once
#include <string.h>
#include <vector>
#include "NPCarContainer.h"
#include "Droga.h"
#include "Pickup.h"

class Background
{
private:
	// Private variables
	sf::Vector2u windowSize;
	float start_dekoracje;
	float scale;
	int *aktualna_lokacja;
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

	static constexpr int liczba_lokacji = 4;
	static constexpr int liczba_typow_dekoracji = 4;
	static constexpr int liczba_typow_drog = 2;

	int* zycia;
	float* mnoznik_predkosci, * mnoznik_punktow, * dystans, * punkty, * predkosc;

	std::vector<std::vector<sf::Texture>> vtexture_dekoracje;
	sf::Texture texture_dekoracje[liczba_lokacji][liczba_typow_dekoracji];

	std::vector<std::vector<sf::Texture>> vtexture_droga;
	sf::Texture texture_droga[liczba_lokacji][liczba_typow_drog];

	std::vector <Droga*> vdroga;

	static constexpr int liczba_pickups = 5;
	sf::Texture *texture_pickups;
	int szansa_dziura, szansa_lod, szansa_pekniecie, szansa_przyspieszenie, szansa_smar;
	int max_dziura, max_lod, max_pekniecie, max_przyspieszenie, max_smar;
	int liczba_dziura{}, liczba_lod{}, liczba_pekniecie{}, liczba_przyspieszenie{}, liczba_smar{};
	std::vector <Pickup*> vpickups;

	NPCarContainer* NPCar;

	// Initialization functions 
	void initVariables(sf::Vector2u windowSize, int* _aktualna_lokacja, float* _dystans, float* _mnoznik_predkosci, float* _mnoznik_punktow, float* _punkty, float* _predkosc, int* _zycia);

	// Private road functions
	float getNajwyzszaDroga();
	void dodajDroge(float wysokosc);

	// Private pickups functions
	void spawnPickups();
	bool collidePickups(sf::FloatRect obj);
	void deletePickups();

public:
	// Constructors / Destructors
	Background(sf::Vector2u windowSize, int *_aktualna_lokacja, float *_dystans, float *_mnoznik_predkosci, float* _mnoznik_punktow, float* _punkty, float* _predkosc, int* _zycia);
	virtual ~Background();

	// Public functions
	bool backgroundContainsV2f(const sf::Vector2f&obj);

	// Update functions
	void update(float dt, float movement_offset);
	void updateRoad(float dt, float movement_offset);
	void updatePickups(float dt, float movement_offset);
	void updateNPCar(float dt);
	void updateStatistics(float dt, float movement_offset);

	// Accessors / Mutators
	sf::FloatRect getMapBorders();
	sf::FloatRect getRoadBorders();
	float getCarSpawnRight();
	float getCarSpawnLeft();
	std::vector <Pickup*>* getPickupsPtr();
	NPCarContainer* getNPCarContainerPtr();
	float getSpeedFactor();
	float getPointsFactor();

	void setSpeedFactor(float value);
	void addSpeedFactor(float value);
	void setPointsFactor(float value);
	void addPointsFactor(float value);

	// Rendering functions
	void render(sf::RenderTarget& target);
};