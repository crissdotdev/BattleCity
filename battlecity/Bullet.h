#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML/Graphics.hpp>
#include "Macros.h"
#include "WorldEntity.h"
#include <algorithm>
#include "Enemy.h"
#include "PowerUps.h"

class Enemy;

class Bullet
{
public:
	Bullet();
	~Bullet();
	Bullet(Position position, char direction, Tank* firedBy, bool firedByEnemy);

	sf::Texture m_bulletTexture;
	sf::Sprite m_bulletSprite;

	Tank* GetFiredBy() const;

	sf::Texture GetTexture();
	Position GetPosition() const;
	bool HandleBullet(std::vector<std::unique_ptr<Bullet>>& bullets, std::vector<std::unique_ptr<WorldEntity>>& worldEntities, std::vector<std::unique_ptr<Enemy>>& enemyTanks, std::vector<PowerUps>& powerUps, std::vector<Position> powerUpSpawnPoints, bool& wallHit, bool& tankHit, bool& solidHit);

protected:
	Position m_bulletPosition;
	char m_bulletDirection = DIR_UP;
	Tank* m_firedBy;
	bool m_firedByEnemy;
};

