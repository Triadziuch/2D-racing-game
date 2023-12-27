#include "NPCarContainer.h"

// Initialization functions 
void NPCarContainer::initVariables(float _car_spawn_left, float _car_spawn_right, sf::Vector2u _windowSize)
{
	this->car_spawn_left	 = _car_spawn_left;
	this->car_spawn_right	 = _car_spawn_right;
	this->windowSize		 = _windowSize;

	this->texture_car = new sf::Texture[this->car_number];
	for (int i = 0; i < this->car_number; ++i)
		if (!this->texture_car[i].loadFromFile("Textures/Samochody/car" + std::to_string(i) + ".png"))
			printf("ERROR: Nie udalo sie wczytac pliku Textures/Samochody/car%d.png\n", i);
}

// Private functions
void NPCarContainer::spawn()
{
	if ((rand() % 50 == 0 && this->vCars.size() == 0) || (rand() % 150 == 0 && this->vCars.size() < this->max_cars)) {
		if (rand() % 2 == 0 && this->last_spawn != 'R') {
			this->last_spawn = 'R';
			this->vCars.push_back(new NPCar(this->texture_car[rand() % this->car_number], static_cast<float>(rand() % 300 + 200), { this->car_spawn_right, static_cast<float>(this->windowSize.y) + 100.f }, -1));
		}
		else if (this->last_spawn != 'L'){
			this->last_spawn = 'L';
			this->vCars.push_back(new NPCar(this->texture_car[rand() % this->car_number], static_cast<float>(rand() % 450 + 380), { this->car_spawn_left, -100.f }, 1));
		}
	}
}

void NPCarContainer::delete_out_of_border()
{
	for (size_t i = 0; i < this->vCars.size(); ++i)
		if (this->vCars[i]->getFloatRect().top < -300.f || this->vCars[i]->getFloatRect().top > this->windowSize.y + 300.f)
			this->vCars.erase(this->vCars.begin() + i);
}

// Constructors / Destructors
NPCarContainer::NPCarContainer(float _car_spawn_left, float _car_spawn_right, sf::Vector2u _windowSize)
{
	srand(static_cast<unsigned>(time(nullptr)));
	this->initVariables(_car_spawn_left, _car_spawn_right, _windowSize);
}

NPCarContainer::~NPCarContainer()
{
	for (auto const& car : this->vCars)
		car->~NPCar();
	this->vCars.clear();

	delete[] this->texture_car;
}

// Update functions
void NPCarContainer::update(float dt)
{
	this->spawn();
	this->delete_out_of_border();

	for (auto& car : this->vCars)
		car->update(dt);
}

// Rendering functions
void NPCarContainer::render(sf::RenderTarget& target)
{
	for (auto &car : this->vCars)
		car->render(target);
}