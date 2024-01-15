#include "Leaderboard.h"

void Leaderboard::initVariables(sf::RenderWindow* _window, float* _points, bool* _isEnd)
{
	this->window = _window;
	this->points = _points;
	this->column_width = 300.f;
	this->filename = "leaderboard.txt";
	this->isEnd = _isEnd;

	this->font = AssetManager::GetFont("Fonts/Pixel.ttf");
	this->font_size = 32u;

	fstream file;
	file.open(this->filename);
	int i = 0;
	while (!file.eof() && i < this->wpisow) {
		file >> names[i] >> leaderboard_points[i];
		++i;
	}	
	file.close();

	this->enteringName = (stoi(this->leaderboard_points[this->wpisow - 1]) < static_cast<int>(*this->points));

	for (int i = 0; i < 3; ++i) {
		this->text_leaderboard[0][i].setFont(this->font);
		this->text_leaderboard[0][i].setCharacterSize(this->font_size + 8u);
		this->text_leaderboard[0][i].setFillColor(sf::Color::White);
		this->text_leaderboard[0][i].setString(this->s_nazwy_kolumn[i]);
	}

	this->text_leaderboard[0][0].setPosition({ this->window->getSize().x / 2.f - this->text_leaderboard[0][0].getGlobalBounds().width - this->column_width }, 100.f);
	this->text_leaderboard[0][1].setPosition({ this->window->getSize().x / 2.f - this->text_leaderboard[0][0].getGlobalBounds().width / 2.f}, 100.f);
	this->text_leaderboard[0][2].setPosition({ this->window->getSize().x / 2.f + + this->column_width }, 100.f);

	for (int i = 1; i < this->wpisow + 1; ++i) {
		this->text_leaderboard[i][0].setFont(this->font);
		this->text_leaderboard[i][0].setCharacterSize(this->font_size);
		this->text_leaderboard[i][0].setFillColor(sf::Color::White);
		this->text_leaderboard[i][0].setString(to_string(i) + '.');
		this->text_leaderboard[i][0].setPosition({ this->text_leaderboard[0][0].getGlobalBounds().left, this->text_leaderboard[i - 1][0].getGlobalBounds().top + this->text_leaderboard[i - 1][0].getGlobalBounds().height * 2 });

		this->text_leaderboard[i][1].setFont(this->font);
		this->text_leaderboard[i][1].setCharacterSize(this->font_size);
		this->text_leaderboard[i][1].setFillColor(sf::Color::White);
		this->text_leaderboard[i][1].setString(this->names[i - 1]);
		this->text_leaderboard[i][1].setPosition({ this->text_leaderboard[0][1].getGlobalBounds().left, this->text_leaderboard[i - 1][1].getGlobalBounds().top + this->text_leaderboard[i - 1][1].getGlobalBounds().height * 2 });

		this->text_leaderboard[i][2].setFont(this->font);
		this->text_leaderboard[i][2].setCharacterSize(this->font_size);
		this->text_leaderboard[i][2].setFillColor(sf::Color::White);
		this->text_leaderboard[i][2].setString(this->leaderboard_points[i - 1]);
		this->text_leaderboard[i][2].setPosition({ this->text_leaderboard[0][2].getGlobalBounds().left, this->text_leaderboard[i - 1][2].getGlobalBounds().top + this->text_leaderboard[i - 1][2].getGlobalBounds().height * 2 });
	}

	this->s_podaj_nazwe = "Podaj nazwe: ";
	this->text_podaj_nazwe.setFont(this->font);
	this->text_podaj_nazwe.setCharacterSize(this->font_size);
	this->text_podaj_nazwe.setFillColor(sf::Color::White);
	this->text_podaj_nazwe.setString(this->s_podaj_nazwe);
	this->text_podaj_nazwe.setPosition({ this->window->getSize().x / 2.f - this->text_podaj_nazwe.getGlobalBounds().width, this->text_leaderboard[this->wpisow][0].getGlobalBounds().top + (this->window->getSize().y - this->text_leaderboard[this->wpisow][0].getGlobalBounds().top) / 2.f });
	
	this->text_nazwa.setFont(this->font);
	this->text_nazwa.setCharacterSize(this->font_size);
	this->text_nazwa.setFillColor(sf::Color::White);
	this->text_nazwa.setPosition({ this->text_podaj_nazwe.getGlobalBounds().left + this->text_podaj_nazwe.getGlobalBounds().width, this->text_podaj_nazwe.getGlobalBounds().top });

	this->s_przejdz_do_menu = "Wcisnij ENTER, aby wrocic do menu...";
	this->text_przejdz_do_menu.setFont(this->font);
	this->text_przejdz_do_menu.setCharacterSize(this->font_size);
	this->text_przejdz_do_menu.setFillColor(sf::Color::White);
	this->text_przejdz_do_menu.setString(this->s_przejdz_do_menu);
	this->text_przejdz_do_menu.setPosition({ this->window->getSize().x / 2.f - this->text_przejdz_do_menu.getGlobalBounds().width / 2.f, this->text_leaderboard[this->wpisow][0].getGlobalBounds().top + (this->window->getSize().y - this->text_leaderboard[this->wpisow][0].getGlobalBounds().top) / 2.f });
}

Leaderboard::Leaderboard(sf::RenderWindow* _window, float* _points, bool* _isEnd)
{
	this->initVariables(_window, _points, _isEnd);
}

Leaderboard::~Leaderboard()
{
}

void Leaderboard::update()
{
	this->updatePollEvents();
}

void Leaderboard::addNameToLeaderboard()
{
	this->enteringName = false;

	for (size_t i = 0; i < this->wpisow; ++i) {
		if (stoi(this->leaderboard_points[i]) < static_cast<int>(*this->points)) {
			for (size_t j = this->wpisow; j > i + 1; --j) {
				this->leaderboard_points[j - 1] = this->leaderboard_points[j - 2];
				this->names[j - 1] = this->names[j - 2];
				
				this->text_leaderboard[j][2].setString(this->leaderboard_points[j - 1]);
				this->text_leaderboard[j][1].setString(this->names[j - 1]);
			}
			this->leaderboard_points[i] = to_string(static_cast<int>(*this->points));
			this->names[i] = this->nazwa_string;
			
			this->text_leaderboard[i + 1][2].setString(this->leaderboard_points[i]);
			this->text_leaderboard[i + 1][1].setString(this->names[i]);

			this->save();
			*this->points = 0;
			break;
		}
	}
}

void Leaderboard::updatePollEvents()
{
	sf::Event ev;

	if (this->window->pollEvent(ev)) {

		// Zamykanie okna
		if ((ev.Event::type == sf::Event::Closed) || (ev.Event::KeyPressed && ev.Event::key.code == sf::Keyboard::Escape)) 
			this->window->close();

		if (ev.type == sf::Event::TextEntered) {

			if (enteringName) {
				if (ev.text.unicode < 128) {
					if (ev.text.unicode != 13) {
						this->nazwa_string += static_cast<char>(ev.text.unicode);
						this->text_nazwa.setString(this->nazwa_string);
					}
					else
						this->addNameToLeaderboard();
				}
			}
		}
		if (!this->enteringName && ev.Event::type == ev.Event::KeyPressed && ev.Event::key.code == sf::Keyboard::Enter)
			*this->isEnd = false;
	}
}

void Leaderboard::save()
{
	ofstream file;
	file.open(this->filename);
	if (file.good()) {
		for (size_t i = 0; i < this->wpisow; ++i)
			file << this->names[i] << ' ' << this->leaderboard_points[i] << '\n';
	}
	else
		printf("ERROR: Couldn't open file: %s\r\n", &this->filename);

	file.close();
}

void Leaderboard::render(sf::RenderTarget& target)
{
	this->window->clear(sf::Color::Black);

	for (int i = 0; i < this->wpisow + 1; ++i) {
		target.draw(this->text_leaderboard[i][0]);
		target.draw(this->text_leaderboard[i][1]);
		target.draw(this->text_leaderboard[i][2]);
	}

	if (this->enteringName) {
		target.draw(this->text_podaj_nazwe);
		target.draw(this->text_nazwa);
	}
	else
		target.draw(this->text_przejdz_do_menu);

	this->window->display();
}
