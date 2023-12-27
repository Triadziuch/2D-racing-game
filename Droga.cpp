#include "Droga.h"

void Droga::initVariables(const sf::Vector2i _rozmiar, float _skala, sf::Vector2f start_dekoracje, sf::Texture* texture_dekoracje, int liczba_dekoracji, bool** rozmieszenie_dekoracji, sf::Texture& texture_droga)
{
	this->skala = _skala;
	this->rozmiar = _rozmiar;

	this->sprite_dekoracje = new sf::Sprite * [this->rozmiar.y];
	for (int i = 0; i < this->rozmiar.y; ++i)
		this->sprite_dekoracje[i] = new sf::Sprite[this->rozmiar.x];

	for (int i = 0; i < this->rozmiar.x; ++i)
		for (int j = 0; j < this->rozmiar.y; ++j) 
			if (rozmieszenie_dekoracji[j][i]) {
				int wybrana_dekoracja = rand() % 100 + 1;
				if (wybrana_dekoracja <= 70)
					wybrana_dekoracja = 0;
				else if (wybrana_dekoracja <= 80)
					wybrana_dekoracja = 1;
				else if (wybrana_dekoracja <= 90)
					wybrana_dekoracja = 2;
				else if (wybrana_dekoracja <= 100)
					wybrana_dekoracja = 3;

				this->sprite_dekoracje[j][i].setTexture(texture_dekoracje[wybrana_dekoracja]);
				this->sprite_dekoracje[j][i].setScale({ this->skala, this->skala });
				this->sprite_dekoracje[j][i].setPosition({ start_dekoracje.x + i * this->sprite_dekoracje[j][i].getGlobalBounds().width, start_dekoracje.y - (this->rozmiar.y - j) * this->sprite_dekoracje[j][i].getGlobalBounds().height  });
			}

	this->sprite_droga.setTexture(texture_droga);
	this->sprite_droga.setScale({ this->skala, this->skala });
	this->sprite_droga.setPosition(start_dekoracje.x, start_dekoracje.y - this->sprite_dekoracje[0][0].getGlobalBounds().width * this->rozmiar.y);
}

Droga::Droga(const sf::Vector2i _rozmiar, float _skala, sf::Vector2f start_dekoracje, sf::Texture* texture_dekoracje, int liczba_dekoracji, bool** rozmieszenie_dekoracji, sf::Texture& texture_droga)
{
	this->initVariables(_rozmiar, _skala, start_dekoracje, texture_dekoracje, liczba_dekoracji, rozmieszenie_dekoracji, texture_droga);
}

Droga::~Droga()
{
}

void Droga::update(float dt)
{
}

void Droga::move(float value)
{
	for (int i = 0; i < this->rozmiar.y; ++i)
		for (int j = 0; j < this->rozmiar.x; ++j)
			this->sprite_dekoracje[i][j].move({ 0.f, value });
	this->sprite_droga.move({ 0.f, value });
}

bool Droga::getDrogaContains(const sf::Vector2f& obj)
{
	for (int i = 0; i < this->rozmiar.y; ++i)
		for (int j = 0; j < this->rozmiar.x; ++j)
			if (this->sprite_dekoracje[i][j].getTexture() != nullptr && this->sprite_dekoracje[i][j].getGlobalBounds().contains(obj))
				return false;
	return true;
}

sf::FloatRect Droga::getBorders()
{
	sf::FloatRect borders;
	borders.left = this->sprite_dekoracje[0][0].getGlobalBounds().left;
	borders.top = this->sprite_dekoracje[0][0].getGlobalBounds().top;
	borders.height = this->sprite_dekoracje[this->rozmiar.y - 1][0].getGlobalBounds().top - this->sprite_dekoracje[0][0].getGlobalBounds().top + this->sprite_dekoracje[0][0].getGlobalBounds().height;
	borders.width = this->sprite_dekoracje[0][this->rozmiar.x - 1].getGlobalBounds().left - this->sprite_dekoracje[0][0].getGlobalBounds().left + this->sprite_dekoracje[0][0].getGlobalBounds().width;
	return borders;
}

float Droga::getPosition()
{
	return this->sprite_dekoracje[0][0].getPosition().y;
}

void Droga::render(sf::RenderTarget& target)
{
	for (int i = 0; i < this->rozmiar.y; ++i)
		for (int j = 0; j < this->rozmiar.x; ++j)
			target.draw(this->sprite_dekoracje[i][j]);
	target.draw(this->sprite_droga);
}
