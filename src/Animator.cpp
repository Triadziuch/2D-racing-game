#include "Animator.h"

Animator::Animation& Animator::CreateAnimation(std::string const& name, std::string const& textureName, sf::Time const& duration, bool loop)
{
	this->m_Animations.emplace_back(name, textureName, duration, loop);

	// If we don't have any other animations, use that as current animation
	if (this->m_CurrentAnimation == nullptr)
		this->SwitchAnimation(&m_Animations.back());

	return m_Animations.back();
}

void Animator::Update(sf::Time const& dt)
{
	// If we don't have any animations yet return
	if (this->m_CurrentAnimation == nullptr)
		return;

	this->m_CurrentTime += dt;

	// Get the current animation frame
	float scaledTime = (this->m_CurrentTime.asSeconds() / this->m_CurrentAnimation->m_Duration.asSeconds());
	int numFrames = this->m_CurrentAnimation->m_Frames.size();
	int currentFrame = static_cast<int>(scaledTime * numFrames);

	// If the animation is looping, calculate the correct frame
	if (this->m_CurrentAnimation->m_Looping)
		currentFrame %= numFrames;
	else if (currentFrame >= numFrames) // If the current frame is greater than the number of frames
		currentFrame = numFrames - 1;	// Show last frame

	// Set the texture rectangle, depending on the frame
	this->m_Sprite.setTextureRect(this->m_CurrentAnimation->m_Frames[currentFrame]);
}

Animator::Animation* Animator::FindAnimation(std::string const& name)
{
	for (auto it = this->m_Animations.begin(); it != this->m_Animations.end(); ++it)
		if (it->m_Name == name)
			return &*it;

	return nullptr;
}

void Animator::SwitchAnimation(Animator::Animation* animation)
{
	// Change the sprite texture
	if (animation != nullptr) {
		m_Sprite.setTexture(AssetManager::GetTexture(animation->m_TextureName));
	}
	this->m_CurrentAnimation = animation;
	this->m_CurrentTime = sf::Time::Zero; // Reset the time
}

bool Animator::SwitchAnimation(std::string const& name)
{
	auto animation = this->FindAnimation(name);
	if (animation != nullptr) {
		this->SwitchAnimation(animation);
		return true;
	}
	return false;
}

std::string Animator::GetCurrentAnimationName() const
{
	if (this->m_CurrentAnimation != nullptr)
		return this->m_CurrentAnimation->m_Name;

	return "";
}

sf::Sprite& Animator::getSprite()
{
	return this->m_Sprite;
}
