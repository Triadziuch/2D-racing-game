#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <time.h>

class Droga
{
private:
	// Initialization functions 
	float skala;
	sf::Vector2i rozmiar;
	sf::Sprite** sprite_dekoracje;
	sf::Sprite sprite_droga;

	void initVariables(const sf::Vector2i _rozmiar, float _skala, sf::Vector2f start_dekoracje, sf::Texture* texture_dekoracje, int liczba_dekoracji, bool** rozmieszenie_dekoracji, sf::Texture& texture_droga);

public:
	// Constructors / Destructors
	Droga(const sf::Vector2i _rozmiar, float _skala, sf::Vector2f start_dekoracje, sf::Texture* texture_dekoracje, int liczba_dekoracji, bool** rozmieszenie_dekoracji, sf::Texture& texture_droga);
	virtual ~Droga();

	// Update functions
	void update(float dt);

	void move(float value);
	bool getDrogaContains(const sf::Vector2f&obj);
	sf::FloatRect getBorders();
	float getTileWidth() { return this->sprite_dekoracje[0][0].getGlobalBounds().width; }

	// Accessors
	float getPosition();

	// Rendering background
	void render(sf::RenderTarget& target);
};