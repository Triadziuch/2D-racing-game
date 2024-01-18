#include "Car.h"

// Initialization functions 
void Car::initVariables(sf::RenderWindow* _window, bool* _isMenu)
{
	this->window = _window;
	this->isMenu = _isMenu;
	this->speed = 30.f;
	this->scale = 4.f;
	this->sprite.setTexture(AssetManager::GetTexture("Textures/Samochody/car4.png"));
	this->sprite.setOrigin(this->sprite.getGlobalBounds().left + this->sprite.getGlobalBounds().width / 2.f, this->sprite.getGlobalBounds().top + this->sprite.getGlobalBounds().height / 2.f);
	this->sprite.setScale({ this->scale, this->scale });
	this->sprite.setPosition(static_cast<sf::Vector2f>(this->window->getSize() / 2u));
	this->speed_factor = 1.f;

	this->RP = new RotationProcessing();
	this->using_mouse = this->RP->getIsMouse();
}

// Constructors / Destructors
Car::Car(sf::RenderWindow* _window, bool* _isMenu)
{
	this->initVariables(_window, _isMenu);
}

Car::~Car()
{
	this->RP->~RotationProcessing();
	this->RP = nullptr;
}

// Update functions
void Car::update(float dt)
{
	if (!this->using_mouse) {
		this->RP->update();
		this->updateMovement(dt);
		return;
	}
	else {
		this->updateMovementMouse();
	}
}

sf::Vector2f Car::updateRotation()
{
	sf::Vector2f rotation = { static_cast<float>(this->RP->getNewLR()), static_cast<float>(this->RP->getNewFB()) };

	float kierunek = signbit(rotation.x) ? -1.f : 1.f;
	if (abs(rotation.x) < 2.f) {
		rotation.x = 0.f;
		this->sprite.setRotation(0.f);
	}
	else if (abs(rotation.x) < 8.f) {
		this->sprite.setRotation(8.f * kierunek);
	}
	else if (abs(rotation.x) < 16.f)
		this->sprite.setRotation(16.f * kierunek);
	else
		this->sprite.setRotation(24.f * kierunek);

	if (abs(rotation.y) < 1.f)
		rotation.y = 0.f;

	return rotation;
}

void Car::updateMovement(float dt)
{
	sf::Vector2f rotation = this->updateRotation();
	this->sprite.move({ rotation.x * dt * this->speed * this->speed_factor, rotation.y * dt * this->speed * this->speed_factor });
}

void Car::updateMovementMouse()
{
	sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*this->window));
	this->sprite.setPosition(mouse_pos);
}

// Public functions
void Car::move(sf::Vector2f _offset)
{
	this->sprite.move(_offset);
}

void Car::move(sf::Vector2f _offset, float _dt)
{
	this->sprite.move(_offset * _dt * this->speed * this->speed_factor);
}

void Car::reset()
{
	this->sprite.setPosition(static_cast<sf::Vector2f>(this->window->getSize() / 2u));
	this->speed_factor = 1.f;
}

// Rendering the game
void Car::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}