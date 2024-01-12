#include "Background.h"

// Initialization functions 
void Background::initVariables(sf::Vector2u windowSize, int* _aktualna_lokacja, float* _dystans, float* _mnoznik_predkosci, float* _mnoznik_punktow, float* _punkty, float* _predkosc, int* _zycia)
{
	this->windowSize = windowSize;
	this->start_dekoracje = static_cast<float>(this->windowSize.x) * 0.3f;
	this->scale = 4.f;
	this->aktualna_lokacja = _aktualna_lokacja;
	this->dystans = _dystans;
	this->mnoznik_predkosci = _mnoznik_predkosci;
	this->mnoznik_punktow = _mnoznik_punktow;
	this->punkty = _punkty;
	this->predkosc = _predkosc;
	this->zycia = _zycia;


	// Texture vectors initialization
	this->vtexture_dekoracje.resize(this->liczba_lokacji);
	for (auto& tex : this->vtexture_dekoracje)
		tex.resize(this->liczba_typow_dekoracji);

	this->vtexture_droga.resize(this->liczba_lokacji);
	for (auto& tex : this->vtexture_droga)
		tex.resize(this->liczba_typow_drog);

	// Road variables initialization
	this->rozmieszenie_dekoracji_droga = new bool* [rozmiar_droga.y];
	for (int i = 0; i < rozmiar_droga.y; ++i)
		this->rozmieszenie_dekoracji_droga[i] = this->dekoracje_droga[i];

	this->rozmieszenie_dekoracji_tankowanie = new bool* [rozmiar_tankowanie.y];
	for (int i = 0; i < rozmiar_tankowanie.y; ++i)
		this->rozmieszenie_dekoracji_tankowanie[i] = this->dekoracje_tankowanie[i];

	for (int i = 0; i < this->liczba_lokacji; ++i)
		for (int j = 0; j < this->liczba_typow_dekoracji; ++j)
			if (!this->texture_dekoracje[i][j].loadFromFile("Textures/Dekoracje/Ziemia" + std::to_string(i) + std::to_string(j) + ".png"))
				printf("ERROR: Nie udalo sie wczytac pliku Textures/Dekoracje/Ziemia%d%d.png\n", i, j);

	for (int i = 0; i < this->liczba_lokacji; ++i)
		for (int j = 0; j < this->liczba_typow_dekoracji; ++j)
			this->vtexture_dekoracje[i][j] = this->texture_dekoracje[i][j];

	for (int i = 0; i < this->liczba_lokacji; ++i)
		for (int j = 0; j < this->liczba_typow_drog; ++j)
			if (!this->texture_droga[i][j].loadFromFile("Textures/Drogi/Droga" + std::to_string(i) + std::to_string(j) + ".png"))
				printf("ERROR: Nie udalo sie wczytac pliku Textures/Drogi/Droga%d%d.png\n", i, j);

	for (int i = 0; i < this->liczba_lokacji; ++i)
		for (int j = 0; j < this->liczba_typow_drog; ++j)
			this->vtexture_droga[i][j] = this->texture_droga[i][j];
			

	// Pickups variables initialization
	this->texture_pickups = new sf::Texture[this->liczba_pickups];
	if (!this->texture_pickups[0].loadFromFile("Textures/Pickups/Dziura.png"))
		printf("ERROR: Nie udalo sie wczytac pliku Textures/Pickups/Dziura.png");

	if (!this->texture_pickups[1].loadFromFile("Textures/Pickups/Lod.png"))
		printf("ERROR: Nie udalo sie wczytac pliku Textures/Pickups/Lod.png");

	if (!this->texture_pickups[2].loadFromFile("Textures/Pickups/Pekniecie.png"))
		printf("ERROR: Nie udalo sie wczytac pliku Textures/Pickups/Pekniecie.png");

	if (!this->texture_pickups[3].loadFromFile("Textures/Pickups/Przyspieszenie.png"))
		printf("ERROR: Nie udalo sie wczytac pliku Textures/Pickups/Przyspieszenie.png");

	if (!this->texture_pickups[4].loadFromFile("Textures/Pickups/Smar.png"))
		printf("ERROR: Nie udalo sie wczytac pliku Textures/Pickups/Smar.png");

	this->szansa_dziura = 5;
	this->szansa_lod = 80;
	this->szansa_pekniecie = 180;
	this->szansa_przyspieszenie = 30;
	this->szansa_smar = 80;

	this->max_dziura = 1;
	this->max_lod = 1;
	this->max_pekniecie = 3;
	this->max_przyspieszenie = 1;
	this->max_smar = 1;

	this->updateRoad(0.f, 0.f);
	this->NPCar = new NPCarContainer(this->getCarSpawnLeft(), this->getCarSpawnRight(), this->windowSize, this->mnoznik_predkosci);
}

// Private road functions
float Background::getNajwyzszaDroga()
{
	if (this->vdroga.size() == 0)
		return static_cast<float>(this->windowSize.y);
	else
		return this->vdroga.back()->getPosition();
}

void Background::dodajDroge(float wysokosc)
{
	if (rand() % 100 + 1  < 3) 
		this->vdroga.push_back(new Droga(this->rozmiar_tankowanie, this->scale, { this->start_dekoracje, wysokosc }, this->vtexture_dekoracje[*this->aktualna_lokacja], this->liczba_typow_dekoracji, this->rozmieszenie_dekoracji_tankowanie, this->vtexture_droga[*this->aktualna_lokacja][1]));
	else
		this->vdroga.push_back(new Droga(this->rozmiar_droga, this->scale, { this->start_dekoracje, wysokosc }, this->vtexture_dekoracje[*this->aktualna_lokacja], this->liczba_typow_dekoracji, this->rozmieszenie_dekoracji_droga, this->vtexture_droga[*this->aktualna_lokacja][0]));
}

// Private pickups functions
void Background::spawnPickups()
{
	int wybor = rand() % 17500;
	int texture_id = -1;
	std::string type;

	if (wybor < this->szansa_dziura && this->liczba_dziura < this->max_dziura) {
		this->liczba_dziura++;
		texture_id = 0;
		type = "dziura";
	}
	else if (wybor < szansa_przyspieszenie && this->liczba_przyspieszenie < this->max_przyspieszenie) {
		this->liczba_przyspieszenie++;
		texture_id = 3;
		type = "przyspieszenie";
	}
	else if (wybor < szansa_smar && this->liczba_smar < this->max_smar) {
		this->liczba_smar++;
		if (*this->aktualna_lokacja == 3){
			texture_id = 1;
			type = "lod";
		}
		else {
			texture_id = 4;
			type = "smar";
		}
	}
	else if (wybor < szansa_pekniecie && this->liczba_pekniecie < this->max_pekniecie) {
		this->liczba_pekniecie++;
		texture_id = 2;
		type = "pekniecie";
	}

	if (texture_id != -1) {
		int car_spawn_left_pos = static_cast<int>(this->getCarSpawnLeft());
		int car_spawn_right_pos = static_cast<int>(this->getCarSpawnRight());

		this->vpickups.push_back(new Pickup({ static_cast<float>(car_spawn_left_pos + rand() %(car_spawn_right_pos - car_spawn_left_pos)) - 48.f, -100.f }, 
											  this->texture_pickups[texture_id], type, static_cast<float>(this->windowSize.x) / 2.f));
		
		
		/*while (this->collidePickups(this->vpickups.back()->getFloatRect())) {
			this->vpickups.pop_back();
			this->vpickups.push_back(new Pickup({ static_cast<float>(car_spawn_right_pos + rand() % (car_spawn_left_pos + 32 - car_spawn_right_pos)) - 48.f, -100.f }, this->texture_pickups[texture_id], type));
		}*/
	}
}

bool Background::collidePickups(sf::FloatRect obj)
{
	for (const auto& pickup : this->vpickups)
		if (pickup->getFloatRect().intersects(obj) && obj != pickup->getFloatRect())
			return true;
	return false;
}

void Background::deletePickups()
{
	for (size_t i = 0; i < this->vpickups.size(); ++i)
		if (this->vpickups[i]->getFloatRect().top > this->windowSize.y) {
			std::string type = this->vpickups[i]->getType();
			if (type == "dziura")
				--this->liczba_dziura;
			else if (type == "przyspieszenie")
				--this->liczba_przyspieszenie;
			else if (type == "lod" || type == "smar")
				--this->liczba_smar;
			else if (type == "pekniecie")
				--this->liczba_pekniecie;
			this->vpickups.erase(this->vpickups.begin() + i);
		}
}

// Constructors / Destructors
Background::Background(sf::Vector2u windowSize, int* _aktualna_lokacja, float* _dystans, float* _mnoznik_predkosci, float* _mnoznik_punktow, float* _punkty, float* _predkosc, int* _zycia)
{
	srand(static_cast<unsigned>(time(NULL)));
	this->initVariables(windowSize, _aktualna_lokacja, _dystans, _mnoznik_predkosci, _mnoznik_punktow, _punkty, _predkosc, _zycia);
}

Background::~Background()
{
	delete[] this->rozmieszenie_dekoracji_droga;
	delete[] this->rozmieszenie_dekoracji_tankowanie;

	for (const auto& droga : this->vdroga)
		droga->~Droga();
	this->vdroga.clear();
}

// Public functions
bool Background::backgroundContainsV2f(const sf::Vector2f& obj)
{
	for (const auto& droga : this->vdroga)
		if (!droga->getDrogaContains(obj))
			return false;
	return true;
}

// Update functions
void Background::update(float dt, float movement_offset)
{
	this->updateRoad(dt, movement_offset);
	this->updatePickups(dt, movement_offset);
	this->updateNPCar(dt);
	this->updateStatistics(dt, movement_offset);
}

void Background::updateRoad(float dt, float movement_offset)
{
	for (const auto& droga : this->vdroga)
		droga->update(dt, movement_offset * *this->mnoznik_predkosci);

	if (this->getNajwyzszaDroga() + 100.f > 0.f)
		this->dodajDroge(this->getNajwyzszaDroga());

	if (!this->vdroga.empty() && this->vdroga[0]->getPosition() > this->windowSize.y)
		this->vdroga.erase(this->vdroga.begin());
}

void Background::updatePickups(float dt, float movement_offset)
{
	for (const auto& pickup : this->vpickups)
		pickup->update(dt, movement_offset * *this->mnoznik_predkosci);

	this->spawnPickups();
	this->deletePickups();
}

void Background::updateNPCar(float dt)
{
	this->NPCar->update(dt);
}

void Background::updateStatistics(float dt, float movement_offset)
{
	float przesuniecie_mapy = dt * movement_offset * *this->mnoznik_predkosci;
	*this->punkty += przesuniecie_mapy * *this->mnoznik_punktow;
	*this->dystans += przesuniecie_mapy / 1000.f;
}

// Accessors / Mutators
sf::FloatRect Background::getMapBorders()
{
	if (!this->vdroga.empty()) {
		sf::FloatRect borders_down = this->vdroga[0]->getMapBorders();
		return sf::FloatRect(borders_down.left, 0.f, borders_down.width, static_cast<float>(this->windowSize.y));
	}
	return sf::FloatRect(0.f, 0.f, static_cast<float>(this->windowSize.x), static_cast<float>(this->windowSize.y));
}

sf::FloatRect Background::getRoadBorders()
{
	if (!this->vdroga.empty()) {
		sf::FloatRect borders = this->vdroga[0]->getRoadBorders();
		return sf::FloatRect(borders.left, 0.f, borders.width, static_cast<float>(this->windowSize.y));
	}
	return sf::FloatRect(0.f, 0.f, static_cast<float>(this->windowSize.x), static_cast<float>(this->windowSize.y));
}

float Background::getCarSpawnLeft()
{
	if (!this->vdroga.empty())
		return this->start_dekoracje + this->vdroga[0]->getTileWidth() * 5.f;
	return 0.f;
}

float Background::getCarSpawnRight()
{
	if (!this->vdroga.empty())
		return static_cast<float>(this->windowSize.x) - (this->vdroga[0]->getTileWidth() * 5.f + this->start_dekoracje);
	return 0.f;
}

std::vector<Pickup*>* Background::getPickupsPtr()
{
	return &this->vpickups;
}

NPCarContainer* Background::getNPCarContainerPtr()
{
	return this->NPCar;
}

void Background::setSpeedFactor(float value)
{
	*this->mnoznik_predkosci = value;
	*this->predkosc = 100.f * *this->mnoznik_predkosci;
}

void Background::addSpeedFactor(float value)
{
	if (*this->mnoznik_predkosci + value > 0.f) {
		*this->mnoznik_predkosci += value;
		*this->predkosc = 100.f * *this->mnoznik_predkosci;
	}
}

float Background::getSpeedFactor()
{
	return *this->mnoznik_predkosci;
}

float Background::getPointsFactor()
{
	return *this->mnoznik_punktow;
}

void Background::setPointsFactor(float value)
{
	*this->mnoznik_punktow = value;
}

void Background::addPointsFactor(float value)
{
	if (*this->mnoznik_punktow + value > 0.f)
		*this->mnoznik_punktow += value;
}

// Rendering functions
void Background::render(sf::RenderTarget& target)
{
	for (const auto& droga : this->vdroga)
		droga->render(target);

	for (const auto& pickup : this->vpickups)
		pickup->render(target);

	this->NPCar->render(target);
}
