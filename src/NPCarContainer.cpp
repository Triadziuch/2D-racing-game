#include "NPCarContainer.h"

// Initialization functions 
void NPCarContainer::initVariables(float _car_spawn_left, float _car_spawn_right, sf::Vector2u _windowSize, float* _speed_multiplier)
{
	this->car_spawn_left	 = _car_spawn_left;
	this->car_spawn_right	 = _car_spawn_right;
	this->windowSize		 = _windowSize;
	this->speed_multiplier	 = _speed_multiplier;

	this->explosion_animation_filename = "Textures/Explosion/explosion-512.png";

	this->texture_car = new sf::Texture[this->car_number];
	for (int i = 0; i < this->car_number; ++i)
		if (!this->texture_car[i].loadFromFile("Textures/Samochody/car" + std::to_string(i) + ".png"))
			printf("ERROR: Nie udalo sie wczytac pliku Textures/Samochody/car%d.png\n", i);

	AssetManager::GetTexture(this->explosion_animation_filename);
}

// Private functions
void NPCarContainer::spawn()
{
	if ((rand() % 50 == 0 && this->vCars.size() == 0) || (rand() % 150 == 0 && this->vCars.size() < static_cast<size_t>(this->max_cars))) {
		if (rand() % 2 == 0 && this->last_spawn != 'R') {
			this->last_spawn = 'R';
			this->vCars.push_back(new NPCar(this->texture_car[rand() % this->car_number], static_cast<float>(rand() % 300 + 200), { this->car_spawn_right, static_cast<float>(this->windowSize.y) + 100.f }, -1, this->speed_multiplier));
		}
		else if (this->last_spawn != 'L'){
			this->last_spawn = 'L';
			this->vCars.push_back(new NPCar(this->texture_car[rand() % this->car_number], static_cast<float>(rand() % 450 + 380), { this->car_spawn_left, -100.f }, 1, this->speed_multiplier));
		}
	}
}

void NPCarContainer::BOOM(NPCar& car)
{
	this->vExplosionSprite.push_back(new sf::Sprite());
	sf::FloatRect carRect = car.getFloatRect();
	this->vExplosionSprite.back()->setScale(0.2f, 0.2f);
	this->vExplosionAnimator.push_back(new Animator(*this->vExplosionSprite.back()));
	auto& explosionAnimation = this->vExplosionAnimator.back()->CreateAnimation("Explosion", this->explosion_animation_filename, sf::seconds(1.f), false);
	explosionAnimation.AddFrames(sf::Vector2i(0, 0), sf::Vector2i(512, 512), 9);
	this->vExplosionAnimator.back()->Update(sf::seconds(0.f));
	this->vExplosionSprite.back()->setPosition({ carRect.left + carRect.width / 2.f - this->vExplosionSprite.back()->getGlobalBounds().width / 2.f, carRect.top + carRect.height / 2.f - this->vExplosionSprite.back()->getGlobalBounds().height / 2.f });
}

void NPCarContainer::delete_out_of_border()
{
	for (size_t i = 0; i < this->vCars.size(); ++i)
		if (this->vCars[i]->getFloatRect().top < -300.f || this->vCars[i]->getFloatRect().top > this->windowSize.y + 300.f)
			this->vCars.erase(this->vCars.begin() + i);
}

// Constructors / Destructors
NPCarContainer::NPCarContainer(float _car_spawn_left, float _car_spawn_right, sf::Vector2u _windowSize, float *_speed_multiplier)
{
	srand(static_cast<unsigned>(time(nullptr)));
	this->initVariables(_car_spawn_left, _car_spawn_right, _windowSize, _speed_multiplier);
}

NPCarContainer::~NPCarContainer()
{
	for (auto const& car : this->vCars)
		car->~NPCar();
	this->vCars.clear();

	delete[] this->texture_car;
}

// Update functions
void NPCarContainer::update(float dt, float movement_offset)
{
	this->updateNPCars(dt);
	this->updateAnimators(dt, movement_offset);
}

void NPCarContainer::updateNPCars(float dt)
{
	this->spawn();

	if (!this->vCars.empty())
		this->delete_out_of_border();

	for (auto& car : this->vCars)
		car->update(dt);
}

void NPCarContainer::updateAnimators(float dt, float movement_offset)
{
	for (const auto& animator : this->vExplosionAnimator) 
		if (animator) {
			animator->Update(sf::seconds(dt));
			animator->getSprite().move({ 0.f, dt * movement_offset });
		}

	for (size_t i = 0; i < this->vExplosionAnimator.size(); ++i)
		if (this->vExplosionAnimator[i]->getIsDone()) {
			this->vExplosionSprite.erase(this->vExplosionSprite.begin() + i);
			this->vExplosionAnimator.erase(this->vExplosionAnimator.begin() + i);
			--i;
		}
}

bool NPCarContainer::intersects(const sf::FloatRect& rectangle) 
{
	for (size_t i = 0; i < this->vCars.size(); ++i)
		if (this->vCars[i]->getFloatRect().intersects(rectangle)){
			this->BOOM(*this->vCars[i]);
			this->vCars.erase(this->vCars.begin() + i);
			return true;
		}
	return false;
}

// Rendering functions
void NPCarContainer::render(sf::RenderTarget& target)
{
	for (auto &car : this->vCars)
		car->render(target);

	for (auto& explosion : this->vExplosionSprite)
		target.draw(*explosion);
}