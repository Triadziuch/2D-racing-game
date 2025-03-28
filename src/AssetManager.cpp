#include "AssetManager.h"
#include <assert.h>

AssetManager* AssetManager::sInstance = nullptr;

AssetManager::AssetManager()
{
	assert(this->sInstance == nullptr);
	sInstance = this;
}

sf::Texture& AssetManager::GetTexture(std::string const& filename)
{
	auto& texMap = sInstance->m_Textures;

	// See if the texture is already loaded
	auto pairFound = texMap.find(filename);
	// If yes, return the texture
	if (pairFound != texMap.end())
		return pairFound->second;
	else {
		// Else, load the texture and return it
		// Create an element in the texture map
		auto& texture = texMap[filename];
		if (!texture.loadFromFile(filename))
			printf("ERROR: Nie udalo sie wczytac pliku %s\n", &filename);
		return texture;
	}
}

sf::Font& AssetManager::GetFont(std::string const& filename)
{
	auto& fontMap = sInstance->m_Fonts;

	// See if the font is already loaded
	auto pairFound = fontMap.find(filename);
	// If yes, return the font
	if (pairFound != fontMap.end())
		return pairFound->second;
	else {
		// Else, load the font and return it
		// Create an element in the font map
		auto& font = fontMap[filename];
		if (!font.loadFromFile(filename))
			printf("ERROR: Nie udalo sie wczytac pliku %s\n", &filename);
		return font;
	}
}

//sf::SoundBuffer& AssetManager::GetSoundBuffer(std::string const& filename)
//{
//	auto& sBufferMap = sInstance->m_SoundBuffers;
//
//	auto pairFound = sBufferMap.find(filename);
//	if (pairFound != sBufferMap.end())
//		return pairFound->second;
//	else {
//		auto& sBuffer = sBufferMap[filename];
//		sBuffer.loadFromFile(filename);
//		return sBuffer;
//	}
//}
