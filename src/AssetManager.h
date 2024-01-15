#pragma once
#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
#include <map>

class AssetManager {
private:
	std::map<std::string, sf::Texture>  m_Textures;
	std::map<std::string, sf::Font>		m_Fonts;
	//std::map<std::string, sf::SoundBuffer> m_SoundBuffers;

	static AssetManager* sInstance;
public:
	AssetManager();
	static sf::Texture& GetTexture(std::string const& filename);
	static sf::Font&	GetFont(std::string const& filename);
	//static sf::SoundBuffer& GetSoundBuffer(std::string const& filename);
};