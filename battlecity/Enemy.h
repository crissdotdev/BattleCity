#pragma once
#include "Tank.h"
#include "Bullet.h"

class Enemy : public Tank
{

public:
	Enemy();
	Enemy(float posX, float posY, bool poweredUp);

	void FireBullet(std::vector<std::unique_ptr<Bullet>>& m_bulletVec, sf::Time elapsed);
	void DoMovement();
	void SetPoweredUp(bool poweredUp);
	bool GetPoweredUp() const;
	float GetEnemySpeed();
	void SetEnemySpeed(float speed);
	bool IsEnemy() const;

protected:
	float m_enemySpeed = 1.2f;
	float m_secondCounter = 0.f;
	bool m_poweredUp = false;

};

