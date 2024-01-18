#include "Loop.h"
#include "Leaderboard.h"
//#include "Transition.h"
//#include <Windows.h>

int main() {
	Loop loop;
	loop.run();

	//sf::RenderWindow* window;
	//sf::Clock dt_clock;

	//window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Scigalka", sf::Style::Fullscreen);
	//window->setFramerateLimit(60);
	//window->setVerticalSyncEnabled(true);
	//Transition trans({0.f, 0.f, static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)});
	////Transition trans({100.f, 100.f, 500.f, 500.f});
	//int i = 0;
	//float dt;
	//while (window->isOpen()) {
	//	++i;
	//	sf::Event ev;
	//	dt = dt_clock.restart().asSeconds();

	//	if (window->pollEvent(ev)) 
	//		if ((ev.Event::type == sf::Event::Closed) || (ev.Event::KeyPressed && ev.Event::key.code == sf::Keyboard::Escape)) 
	//			window->close();

	//	if (i > 100)
	//		trans.move(dt * -2000.f);

	//	window->clear(sf::Color::White);
	//	window->display();
	//}

	return 0;
}