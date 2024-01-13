#pragma once
#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
#include <map>

class AssetManager {
private:
	std::map<std::string, sf::Texture> m_Textures;
	//std::map<std::string, sf::SoundBuffer> m_SoundBuffers;

	static AssetManager* sInstance;
public:
	AssetManager();
	static sf::Texture& GetTexture(std::string const& filename);
	//static sf::SoundBuffer& GetSoundBuffer(std::string const& filename);
};