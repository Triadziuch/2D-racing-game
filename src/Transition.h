#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <math.h>

typedef struct Transition {
	sf::FloatRect roadSize;
	float wielkosc_zabka = 20.f;
	std::vector<sf::CircleShape*> zabki_gora, zabki_dol;
	sf::RectangleShape filling;

	Transition(sf::FloatRect _roadSize) {
		this->roadSize = _roadSize;
		int ilosc_zabkow = static_cast<int>(ceil(this->roadSize.width / this->wielkosc_zabka / 2.f));

		this->filling.setSize({ this->roadSize.width, this->roadSize.height + 100.f });
		this->filling.setPosition({ this->roadSize.left, this->roadSize.top });
		this->filling.setFillColor(sf::Color::Black);
		this->filling.setOutlineThickness(0.f);

		this->zabki_gora.push_back(new sf::CircleShape(this->wielkosc_zabka, 3));
		//this->zabki_gora.back()->setOrigin({ this->zabki_gora.back()->getPosition().x + this->zabki_gora.back()->getGlobalBounds().width / 2.f, this->zabki_gora.back()->getPosition().y + this->zabki_gora.back()->getGlobalBounds().height / 2.f});
		this->zabki_gora.back()->setPosition(this->filling.getGlobalBounds().left - this->zabki_gora.back()->getGlobalBounds().width / 2.f, this->filling.getGlobalBounds().top - this->zabki_gora.back()->getGlobalBounds().height);
		this->zabki_gora.back()->setFillColor(sf::Color::Black);

		while (this->zabki_gora.back()->getGlobalBounds().left + this->zabki_gora.back()->getGlobalBounds().width < this->roadSize.left + this->roadSize.width) {
			this->zabki_gora.push_back(new sf::CircleShape(this->wielkosc_zabka, 3));
			this->zabki_gora.back()->setPosition(this->zabki_gora[this->zabki_gora.size() - 2]->getGlobalBounds().left + this->zabki_gora[this->zabki_gora.size() - 2]->getGlobalBounds().width, this->zabki_gora[this->zabki_gora.size() - 2]->getGlobalBounds().top);
			this->zabki_gora.back()->setFillColor(sf::Color::Black);
		}

		this->zabki_dol.push_back(new sf::CircleShape(this->wielkosc_zabka, 3));
		this->zabki_dol.back()->setOrigin({ this->zabki_dol.back()->getGlobalBounds().left + this->zabki_dol.back()->getGlobalBounds().width / 2.f, this->zabki_dol.back()->getGlobalBounds().top + this->zabki_dol.back()->getGlobalBounds().height / 2.f });
		this->zabki_dol.back()->setPosition(this->filling.getGlobalBounds().left, this->filling.getGlobalBounds().top + this->filling.getGlobalBounds().height + this->zabki_dol.back()->getGlobalBounds().height / 2.f);
		this->zabki_dol.back()->setRotation(180.f);
		this->zabki_dol.back()->setFillColor(sf::Color::Black);

		while (this->zabki_dol.back()->getGlobalBounds().left + this->zabki_dol.back()->getGlobalBounds().width < this->roadSize.left + this->roadSize.width) {
			this->zabki_dol.push_back(new sf::CircleShape(this->wielkosc_zabka, 3));
			this->zabki_dol.back()->setOrigin({ this->zabki_dol.back()->getGlobalBounds().left + this->zabki_dol.back()->getGlobalBounds().width / 2.f, this->zabki_dol.back()->getGlobalBounds().top + this->zabki_dol.back()->getGlobalBounds().height / 2.f });
			this->zabki_dol.back()->setRotation(180.f);
			this->zabki_dol.back()->setPosition(this->zabki_dol[this->zabki_dol.size() - 2]->getPosition().x + this->zabki_dol[this->zabki_dol.size() - 2]->getGlobalBounds().width, this->zabki_dol[this->zabki_dol.size() - 2]->getPosition().y);
			this->zabki_dol.back()->setFillColor(sf::Color::Black);
		}

		

		this->move(this->roadSize.height + this->zabki_gora[0]->getGlobalBounds().height);
	};

	void move(float offset) {
		for (auto &i : this->zabki_gora)
			i->move({ 0.f, offset });

		for (auto &i : this->zabki_dol)
			i->move({ 0.f, offset });

		this->filling.move({ 0.f, offset });
	}
	bool getCoversScreen() { return (this->filling.getGlobalBounds().top < this->roadSize.top); }
	bool getIsDone() { return this->zabki_dol[0]->getGlobalBounds().top + this->zabki_dol[0]->getGlobalBounds().height < this->roadSize.top; }

	void render(sf::RenderTarget& target) {
		target.draw(this->filling);
		for(const auto &i : this->zabki_gora)
			target.draw(*i);

		for (const auto &i : this->zabki_dol)
			target.draw(*i);
	}
};