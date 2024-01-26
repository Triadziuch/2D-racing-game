#include "Menu.h"

void Menu::initVariables(sf::RenderWindow* _window, bool *_isMenu)
{
	this->window = _window;
	this->isMenu = _isMenu;
	this->isTworcy = false;

	this->logo_sprite.setTexture(AssetManager::GetTexture("Textures/Menu/Logo.png"));
	this->logo_sprite.setScale({ 4.f, 4.f });
	this->logo_sprite.setPosition({ (this->window->getSize().x - this->logo_sprite.getGlobalBounds().width) / 2.f, this->logo_sprite.getGlobalBounds().height / 3.f});

	this->pb_sprite.setTexture(AssetManager::GetTexture("Textures/Menu/PB.png"));
	this->pb_sprite.setPosition({ (this->window->getSize().x - this->pb_sprite.getGlobalBounds().width) / 2.f, this->window->getSize().y - this->pb_sprite.getGlobalBounds().height * 1.5f });

	this->font = &AssetManager::GetFont("Textures/Pixel.ttf");
	this->font_size = 32u;
	this->s_text_menu = new string[this->liczba_opcji]{ "Zwykly", "Czasowy", "Leaderboard", "Tworcy" };
	this->text_menu = new sf::Text[this->liczba_opcji];
	
	this->text_menu[0].setFont(*this->font);
	this->text_menu[0].setCharacterSize(this->font_size);
	this->text_menu[0].setFillColor(sf::Color::White);
	this->text_menu[0].setString(this->s_text_menu[0]);
	this->text_menu[0].setPosition({ this->window->getSize().x / 2.f - this->text_menu[0].getGlobalBounds().width / 2.f, this->logo_sprite.getGlobalBounds().top + this->logo_sprite.getGlobalBounds().height + this->text_menu[0].getGlobalBounds().height * 5});

	for (size_t i = 1; i < this->liczba_opcji; ++i) {
		this->text_menu[i].setFont(*this->font);
		this->text_menu[i].setCharacterSize(this->font_size);
		this->text_menu[i].setFillColor(sf::Color::White);
		this->text_menu[i].setString(this->s_text_menu[i]);
		this->text_menu[i].setPosition({ this->window->getSize().x / 2.f - this->text_menu[i].getGlobalBounds().width / 2.f, this->text_menu[i - 1].getGlobalBounds().top + this->text_menu[i - 1].getGlobalBounds().height * 2.f });
	}

	this->cursor_sprite.setTexture(AssetManager::GetTexture("Textures/Samochody/car4.png"));
	this->cursor_sprite.setScale({ 4.f, 4.f });
	this->cursor_sprite.setRotation(90.f);
	this->cursor_sprite.setPosition({ this->text_menu[this->wybrana_opcja].getGlobalBounds().left - this->cursor_sprite.getGlobalBounds().width, this->text_menu[this->wybrana_opcja].getGlobalBounds().top + this->text_menu[this->wybrana_opcja].getGlobalBounds().height / 2.f - this->cursor_sprite.getGlobalBounds().height / 2.f });

	this->text_tworcy.setFont(*this->font);
	this->text_tworcy.setCharacterSize(this->font_size);
	this->text_tworcy.setFillColor(sf::Color::White);
	this->text_tworcy.setString("Jakub Kirylowicz");
	this->text_tworcy.setPosition({ this->window->getSize().x / 2.f - this->text_tworcy.getGlobalBounds().width / 2.f, this->window->getSize().y / 2.f - this->text_tworcy.getGlobalBounds().height / 2.f });

	/*this->text_tworcy.setFont(*this->font);
	this->text_tworcy.setCharacterSize(this->font_size);
	this->text_tworcy.setFillColor(sf::Color::White);
	this->text_tworcy.setString("Jakub Kazimiruk");
	this->text_tworcy.setPosition({ this->window->getSize().x / 2.f - this->text_tworcy[0].getGlobalBounds().width / 2.f, this->text_tworcy[1].getGlobalBounds().top - this->text_tworcy[0].getGlobalBounds().height * 3.f});*/

	/*this->text_tworcy[2].setFont(*this->font);
	this->text_tworcy[2].setCharacterSize(this->font_size);
	this->text_tworcy[2].setFillColor(sf::Color::White);
	this->text_tworcy[2].setString("Lukasz Kotowski");
	this->text_tworcy[2].setPosition({ this->window->getSize().x / 2.f - this->text_tworcy[2].getGlobalBounds().width / 2.f, this->text_tworcy[1].getGlobalBounds().top + this->text_tworcy[2].getGlobalBounds().height * 2.8f });*/
}

Menu::Menu(sf::RenderWindow* _window, bool* _isMenu)
{
	this->initVariables(_window, _isMenu);
}

Menu::~Menu()
{
}

bool Menu::update()
{
	return this->updatePollEvents();
}

bool Menu::updatePollEvents()
{
	sf::Event ev;

	if (this->window->pollEvent(ev)) {

		// Zamykanie okna
		if ((ev.Event::type == sf::Event::Closed) || (ev.Event::KeyPressed && ev.Event::key.code == sf::Keyboard::Escape)) {
			this->window->close();
		}

		// Wybor opcji
		if (!this->isTworcy) {
			if (ev.Event::type == ev.Event::KeyPressed && !this->zmieniono) {
				this->zmieniono = true;
				if (ev.Event::key.code == sf::Keyboard::Up || ev.Event::key.code == sf::Keyboard::W) {
					if (--this->wybrana_opcja < 0)
						this->wybrana_opcja = this->liczba_opcji - 1;
					this->cursor_sprite.setPosition({ this->text_menu[this->wybrana_opcja].getGlobalBounds().left - this->cursor_sprite.getGlobalBounds().width, this->text_menu[this->wybrana_opcja].getGlobalBounds().top + this->text_menu[this->wybrana_opcja].getGlobalBounds().height / 2.f - this->cursor_sprite.getGlobalBounds().height / 2.f });
				}
				else if (ev.Event::key.code == sf::Keyboard::Down || ev.Event::key.code == sf::Keyboard::S) {
					if (++this->wybrana_opcja >= this->liczba_opcji)
						this->wybrana_opcja = 0;
					this->cursor_sprite.setPosition({ this->text_menu[this->wybrana_opcja].getGlobalBounds().left - this->cursor_sprite.getGlobalBounds().width, this->text_menu[this->wybrana_opcja].getGlobalBounds().top + this->text_menu[this->wybrana_opcja].getGlobalBounds().height / 2.f - this->cursor_sprite.getGlobalBounds().height / 2.f });
				}
				else if (ev.Event::key.code == sf::Keyboard::Enter && this->wybrana_opcja != 3)
					return true;
			}

			if (ev.Event::type == ev.Event::KeyPressed && ev.Event::key.code == sf::Keyboard::Enter && this->wybrana_opcja == 3)
				this->isTworcy = true;

			if (ev.Event::type == ev.Event::KeyReleased)
				this->zmieniono = false;
		}
		else {
			if (ev.Event::type == ev.Event::KeyPressed && ev.Event::key.code == sf::Keyboard::Enter)
				this->isTworcy = false;
		}
	}

	return false;
}

void Menu::render()
{
	this->window->clear();

	if (!this->isTworcy) {
		this->window->draw(this->logo_sprite);
		this->window->draw(this->pb_sprite);

		for (size_t i = 0; i < liczba_opcji; ++i)
			this->window->draw(this->text_menu[i]);

		this->window->draw(this->cursor_sprite);
	}
	else {
		this->window->draw(this->text_tworcy);
	}

	this->window->display();
}
