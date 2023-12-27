#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <time.h>

class Droga
{
private:
	// Private variables
	float scale;
	sf::Vector2i rozmiar;
	sf::Sprite** sprite_dekoracje;
	sf::Sprite sprite_droga;

	// Initialization functions 
	void initVariables(const sf::Vector2i _rozmiar, float _scale, sf::Vector2f _start_dekoracje, sf::Texture* _texture_dekoracje, int _liczba_dekoracji, bool** _rozmieszenie_dekoracji, sf::Texture& _texture_droga);

	// Private functions
	void move(float value);

public:
	// Constructors / Destructors
	Droga(const sf::Vector2i _rozmiar, float _scale, sf::Vector2f _start_dekoracje, sf::Texture* _texture_dekoracje, int _liczba_dekoracji, bool** _rozmieszenie_dekoracji, sf::Texture& _texture_droga);
	virtual ~Droga();

	// Update functions
	void update(float dt, float movement_offset);

	// Accessors / Mutators
	sf::FloatRect getBorders();
	bool getDrogaContains(const sf::Vector2f&obj);
	float getPosition() { return this->sprite_dekoracje[0][0].getPosition().y; }
	float getTileWidth() { return this->sprite_dekoracje[0][0].getGlobalBounds().width; }

	// Rendering functions
	void render(sf::RenderTarget& target);
};