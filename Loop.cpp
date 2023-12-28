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
	this->car = new Car(this->window->getSize());
	this->background = new Background(this->window->getSize());
	this->background_moving_speed = 300.f;
	this->collisionProcessing = new CollisionProcessing(this->car, this->background, this->background->getBorders());
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

	// Displaying frame
	this->window->display();
}