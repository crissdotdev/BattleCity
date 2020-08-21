#include "PowerUps.h"



PowerUps::PowerUps(powerUp type, float posX, float posY): m_powerUp(type), m_powerUpPosition(std::make_pair(posX, posY))
{
	switch (type) {
	case powerUp::BombPU:
	{
		m_powerUpTexture.loadFromFile("../resources/BombPUTexture.png");
		break;
	}
	case powerUp::ClockPU:
	{
		m_powerUpTexture.loadFromFile("../resources/ClockPUTexture.png");
		break;
	}
	case powerUp::ShieldPU:	
	{
		m_powerUpTexture.loadFromFile("../resources/ShieldPUTexture.png");
		break;
	}
	case powerUp::ShovelPU:
	{
		m_powerUpTexture.loadFromFile("../resources/ShovelPUTexture.png");
		break;
	}
	case powerUp::StarPU:
	{
		m_powerUpTexture.loadFromFile("../resources/StarPUTexture.png");
		break;
	}
	case powerUp::TankPU:
	{
		m_powerUpTexture.loadFromFile("../resources/TankPUTexture.png");
		break;
	}
	}

	m_powerUpSprite.setTexture(m_powerUpTexture);
	m_powerUpSprite.setOrigin(sf::Vector2f(m_powerUpTexture.getSize().x * 0.5, m_powerUpTexture.getSize().y * 0.5));
	m_powerUpSprite.setPosition(m_powerUpPosition.first, m_powerUpPosition.second);

}
powerUp PowerUps::GetType() const
{
	return m_powerUp;
}

sf::Sprite PowerUps::GetSprite() const
{
	return m_powerUpSprite;
}
