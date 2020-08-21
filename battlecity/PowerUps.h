#pragma once
#include <SFML\Graphics.hpp>
#include "Macros.h"

enum powerUp {
	BombPU,
	ClockPU,
	ShieldPU,
	ShovelPU,
	StarPU,
	TankPU
};

class PowerUps
{
public:
	PowerUps(powerUp type, float posX, float posY);
	powerUp GetType() const;
	sf::Sprite GetSprite() const;

private:
	sf::Texture m_powerUpTexture;
	sf::Sprite m_powerUpSprite;
	powerUp m_powerUp;
	Position m_powerUpPosition;
};

