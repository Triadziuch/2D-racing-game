#include "Loop.h"

// Initialization functions 
void Loop::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Scigalka", sf::Style::Fullscreen);
	this->window->setFramerateLimit(60);
	this->window->setVerticalSyncEnabled(true);
}

void Loop::initVariables()
{
	srand(static_cast<unsigned>(time(nullptr)));

	this->isEnd = false;
	this->color_background = sf::Color(250, 248, 239);
	this->background_moving_speed = 300.f;

	this->aktualna_lokacja = 2;
	this->zycia_max = 5;
	this->zycia = 5;
	this->dystans = 0.f;
	this->mnoznik_predkosci = 1.f;
	this->mnoznik_puntkow = 1.f;
	this->punkty = 0.f;
	this->predkosc = 100.f;

	this->car = new Car(this->window->getSize());
	this->background = new Background(this->window->getSize(), &this->aktualna_lokacja, &this->dystans, &this->mnoznik_predkosci, &this->mnoznik_puntkow, &this->punkty, &this->predkosc, &this->zycia);
	this->collisionProcessing = new CollisionProcessing(this->car, this->background, this->background->getBorders());
	this->gui = new GUI({ static_cast<float>(this->window->getSize().x), static_cast<float>(this->window->getSize().y) }, this->background->getBorders(), this->nazwy_lokacji,  &this->aktualna_lokacja, &this->punkty, &this->mnoznik_puntkow, &this->dystans, &this->predkosc, &this->zycia_max, &this->zycia);
}

// Constructors / Destructors
Loop::Loop()
{
	this->initWindow();
	this->initVariables();
}

Loop::~Loop()
{
	delete this->car;
	delete this->background;
	delete this->collisionProcessing;
	delete this->window;
}

// Run functions
void Loop::run()
{
	while (this->window->isOpen()) {
		this->update();
		this->render();
	}
}

// Update functions
void Loop::update() {
	this->dt = dt_clock.restart().asSeconds();

	this->updatePollEvents();
	if (!this->isEnd) {
		this->car->update(this->dt);
		this->background->update(this->dt, this->background_moving_speed);
		this->collisionProcessing->update(this->dt);
		this->gui->update();
	}
}

void Loop::updatePollEvents()
{
	sf::Event ev;

	if (this->window->pollEvent(ev)) {

		// Zamykanie okna
		if ((ev.Event::type == sf::Event::Closed) || (ev.Event::KeyPressed && ev.Event::key.code == sf::Keyboard::Escape)) {
			this->window->close();
		}
				
		if (!this->isEnd) {
			if (ev.type == sf::Event::KeyPressed) {

			}
		}
	}
}

// Rendering functions
void Loop::render() {

	this->window->clear(sf::Color::Black);

	this->background->render(*this->window);
	this->car->render(*this->window);
	this->gui->render(*this->window);
	// Displaying frame
	this->window->display();
}