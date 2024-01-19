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
	this->isMenu = true;
	this->isPlaying = false;

	this->color_background = sf::Color(250, 248, 239);
	this->background_moving_speed = 300.f;

	this->aktualna_lokacja = 0;
	this->zycia_max = 5;
	this->zycia = 5;
	this->dystans = 0.f;
	this->mnoznik_predkosci = 1.f;
	this->mnoznik_puntkow = 1.f;
	this->punkty = 0.f;
	this->predkosc = 100.f;

	this->text_test.setFont(this->font);
	this->text_test.setCharacterSize(32u);
	this->text_test.setFillColor(sf::Color::White);
	this->text_test.setPosition({ 20.f, 20.f });

	this->car = new Car(this->window, &this->isMenu);
	this->car->getRP()->initLCDVariables(&this->dystans, &this->punkty, &this->predkosc, &this->zycia, &this->isPlaying);
	this->background = new Background(this->window->getSize(), &this->aktualna_lokacja, &this->dystans, &this->mnoznik_predkosci, &this->mnoznik_puntkow, &this->punkty, &this->predkosc, &this->zycia);
	this->collisionProcessing = new CollisionProcessing(this->car, this->background, this->background->getMapBorders(), this->background->getRoadBorders(), &this->aktualna_lokacja, &this->zycia);
	this->gui = new GUI({ static_cast<float>(this->window->getSize().x), static_cast<float>(this->window->getSize().y) }, this->background->getMapBorders(), this->nazwy_lokacji,  &this->aktualna_lokacja, &this->punkty, &this->mnoznik_puntkow, &this->dystans, &this->predkosc, &this->zycia_max, &this->zycia, &this->isEnd);

	this->window->setMouseCursorVisible(false);
}

void Loop::initAssets()
{
	// Load fonts
	this->font = AssetManager::GetFont("Fonts/Pixel.ttf");

	// Load menu textures
	AssetManager::GetTexture("Textures/Menu/Logo.png");
	AssetManager::GetTexture("Textures/Menu/PB.png");

	// Load background textures
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			AssetManager::GetTexture("Textures/Dekoracje/Ziemia" + to_string(i) + to_string(j) + ".png");

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 2; ++j)
			AssetManager::GetTexture("Textures/Drogi/Droga" + to_string(i) + to_string(j) + ".png");

	AssetManager::GetTexture("Textures/Pickups/Dziura.png");
	AssetManager::GetTexture("Textures/Pickups/Lod.png");
	AssetManager::GetTexture("Textures/Pickups/Pekniecie.png");
	AssetManager::GetTexture("Textures/Pickups/Przyspieszenie.png");
	AssetManager::GetTexture("Textures/Pickups/Smar.png");

	// Load car textures
	for (int i = 0; i <= 8; ++i)
		AssetManager::GetTexture("Textures/Samochody/car" + to_string(i) + ".png");

	// Load hear textures
	AssetManager::GetTexture("Textures/Serce/Serce-puste.png");
	AssetManager::GetTexture("Textures/Serce/Serce-pelne.png");

	// Animation textures
	AssetManager::GetTexture("Textures/Explosion/explosion-512.png");
}

void Loop::initNewGame()
{
	this->isEnd = false;
	this->isMenu = false;
	this->isPlaying = true;

	this->aktualna_lokacja = 0;
	this->zycia = 5;
	this->dystans = 0.f;
	this->mnoznik_predkosci = 1.f;
	this->mnoznik_puntkow = 1.f;
	this->punkty = 0.f;
	this->predkosc = 100.f;

	if (this->car != nullptr)
		this->car->reset();
	else {
		this->car = new Car(this->window, &this->isMenu);
		this->car->getRP()->initLCDVariables(&this->dystans, &this->punkty, &this->predkosc, &this->zycia, &this->isPlaying);
	}
	this->background = new Background(this->window->getSize(), &this->aktualna_lokacja, &this->dystans, &this->mnoznik_predkosci, &this->mnoznik_puntkow, &this->punkty, &this->predkosc, &this->zycia);
	this->collisionProcessing = new CollisionProcessing(this->car, this->background, this->background->getMapBorders(), this->background->getRoadBorders(), &this->aktualna_lokacja, &this->zycia);
	this->gui = new GUI({ static_cast<float>(this->window->getSize().x), static_cast<float>(this->window->getSize().y) }, this->background->getMapBorders(), this->nazwy_lokacji, &this->aktualna_lokacja, &this->punkty, &this->mnoznik_puntkow, &this->dystans, &this->predkosc, &this->zycia_max, &this->zycia, &this->isEnd);
}

// Constructors / Destructors
Loop::Loop()
{
	this->initWindow();
	this->initAssets();
	this->initVariables();
}

Loop::~Loop()
{
	delete this->car;
	delete this->background;
	delete this->collisionProcessing;
	delete this->gui;
	delete this->window;
}

// Run functions
void Loop::run()
{
	this->menu = new Menu(this->window, &this->isMenu);

	while (this->window->isOpen()) {
		this->update();
	}
}

// Update functions
void Loop::update() {
	this->dt = dt_clock.restart().asSeconds();

	if (this->isMenu)
		this->updateMenu();
	else {
		this->updateGameOver();
		this->updatePollEvents();
		if (!this->isEnd) {
			this->updateGame();
			this->render();
		}
		else
			this->updateLeaderboard();
	}
}

void Loop::updateMenu()
{
	while (this->isMenu) {
		this->car->update(0.f);
		if (this->menu->update()) {
			int wybor = this->menu->getOpcja();

			switch (wybor) {
			case 0:
			case 1:
			case 3:
				this->initNewGame();
				break;
			case 2:
				this->isEnd = true;
				this->updateLeaderboard();
				break;
			}
		}
		else {
			this->menu->render();
			this->dt = dt_clock.restart().asSeconds();
		}
	}
}

void Loop::updateGame()
{
	this->car->update(this->dt);
	this->background->update(this->dt, this->background_moving_speed);
	this->collisionProcessing->update(this->dt);
	this->gui->update();
}

void Loop::updateLeaderboard()
{
	this->leaderboard = new Leaderboard(this->window, &this->punkty, &this->isEnd);
	while (this->isEnd) {
		this->leaderboard->update();
		this->car->getRP()->update();
		this->leaderboard->render(*this->window);
		this->dt = dt_clock.restart().asSeconds();
	}
	delete this->leaderboard;
	this->leaderboard = nullptr;
	this->isMenu = true;
}

void Loop::updateGameOver()
{
	if (this->zycia <= 0) {

		this->isEnd = true;
		this->isPlaying = false;
		delete this->background;
		this->background = nullptr;
		delete this->collisionProcessing;
		this->collisionProcessing = nullptr;
		delete this->gui;
		this->gui = nullptr;
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
	this->window->draw(this->text_test);

	this->background->renderTransition(*this->window);

	// Displaying frame
	this->window->display();
}
