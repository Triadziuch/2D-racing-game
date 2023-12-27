#include "Droga.h"

// Initialization functions 
void Droga::initVariables(const sf::Vector2i _rozmiar, float _scale, sf::Vector2f _start_dekoracje, sf::Texture* _texture_dekoracje, int _liczba_dekoracji, bool** _rozmieszenie_dekoracji, sf::Texture& _texture_droga)
{
	this->scale		 = _scale;
	this->rozmiar	 = _rozmiar;

	this->sprite_dekoracje = new sf::Sprite * [this->rozmiar.y];
	for (int i = 0; i < this->rozmiar.y; ++i)
		this->sprite_dekoracje[i] = new sf::Sprite[this->rozmiar.x];

	for (int i = 0; i < this->rozmiar.x; ++i)
		for (int j = 0; j < this->rozmiar.y; ++j) 
			if (_rozmieszenie_dekoracji[j][i]) {
				int wybrana_dekoracja = rand() % 100 + 1;
				if (wybrana_dekoracja <= 70)
					wybrana_dekoracja = 0;
				else if (wybrana_dekoracja <= 80)
					wybrana_dekoracja = 1;
				else if (wybrana_dekoracja <= 90)
					wybrana_dekoracja = 2;
				else if (wybrana_dekoracja <= 100)
					wybrana_dekoracja = 3;

				this->sprite_dekoracje[j][i].setTexture(_texture_dekoracje[wybrana_dekoracja]);
				this->sprite_dekoracje[j][i].setScale({ this->scale, this->scale });
				this->sprite_dekoracje[j][i].setPosition({ _start_dekoracje.x + i * this->sprite_dekoracje[j][i].getGlobalBounds().width, _start_dekoracje.y - (this->rozmiar.y - j) * this->sprite_dekoracje[j][i].getGlobalBounds().height  });
			}

	this->sprite_droga.setTexture(_texture_droga);
	this->sprite_droga.setScale({ this->scale, this->scale });
	this->sprite_droga.setPosition(_start_dekoracje.x, _start_dekoracje.y - this->sprite_dekoracje[0][0].getGlobalBounds().width * this->rozmiar.y);
}

// Private functions
void Droga::move(float value)
{
	for (int i = 0; i < this->rozmiar.y; ++i)
		for (int j = 0; j < this->rozmiar.x; ++j)
			this->sprite_dekoracje[i][j].move({ 0.f, value });
	this->sprite_droga.move({ 0.f, value });
}

// Constructors / Destructors
Droga::Droga(const sf::Vector2i _rozmiar, float _scale, sf::Vector2f _start_dekoracje, sf::Texture* _texture_dekoracje, int _liczba_dekoracji, bool** _rozmieszenie_dekoracji, sf::Texture& _texture_droga)
{
	this->initVariables(_rozmiar, _scale, _start_dekoracje, _texture_dekoracje, _liczba_dekoracji, _rozmieszenie_dekoracji, _texture_droga);
}

Droga::~Droga()
{
}

// Update functions
void Droga::update(float dt, float movement_offset)
{
	this->move(dt * movement_offset);
}

// Accessors / Mutators
sf::FloatRect Droga::getBorders()
{
	sf::FloatRect borders;
	borders.left = this->sprite_dekoracje[0][0].getGlobalBounds().left;
	borders.top = this->sprite_dekoracje[0][0].getGlobalBounds().top;
	borders.height = this->sprite_dekoracje[this->rozmiar.y - 1][0].getGlobalBounds().top - this->sprite_dekoracje[0][0].getGlobalBounds().top + this->sprite_dekoracje[0][0].getGlobalBounds().height;
	borders.width = this->sprite_dekoracje[0][this->rozmiar.x - 1].getGlobalBounds().left - this->sprite_dekoracje[0][0].getGlobalBounds().left + this->sprite_dekoracje[0][0].getGlobalBounds().width;
	return borders;
}

bool Droga::getDrogaContains(const sf::Vector2f& obj)
{
	for (int i = 0; i < this->rozmiar.y; ++i)
		for (int j = 0; j < this->rozmiar.x; ++j)
			if (this->sprite_dekoracje[i][j].getTexture() != nullptr && this->sprite_dekoracje[i][j].getGlobalBounds().contains(obj))
				return false;
	return true;
}

// Rendering functions
void Droga::render(sf::RenderTarget& target)
{
	for (int i = 0; i < this->rozmiar.y; ++i)
		for (int j = 0; j < this->rozmiar.x; ++j)
			target.draw(this->sprite_dekoracje[i][j]);
	target.draw(this->sprite_droga);
}
