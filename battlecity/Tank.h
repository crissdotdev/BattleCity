#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include "Macros.h"

enum tankType {
	enemyTank,
	localPlayerTank
};

class Tank
{
public:
	Tank();
	~Tank();
	Tank(float posX, float posY);

	Position GetPosition() const;
	char GetTankDirection();
	void SetTankDirection(char direction);

	float GetTankSpeed();
	void SetTankSpeed(float speed);
	virtual bool IsEnemy() const;

public:
	sf::Texture m_tankTexture;
	sf::Sprite m_tankSprite;

protected:

	Position m_tankPosition;
	Position m_tankLastNonCollidedPosition;
	char m_tankDirection = DIR_UP;
	float m_tankSpeed = 1.7f;
};

