#include "Enemy.h"
#include "Engine.h"
#include <ctime>


Enemy::Enemy()
{
}

Enemy::Enemy(float posX, float posY, bool poweredUp) : m_poweredUp(poweredUp)
{
	m_tankPosition = std::make_pair(posX, posY);

	m_tankTexture.loadFromFile( m_poweredUp ? "../resources/enemyRed.png" : "../resources/enemy.png");

	m_tankSprite.setTexture(m_tankTexture);
	m_tankTexture.setSmooth(true);
	m_tankSprite.setOrigin(sf::Vector2f(m_tankTexture.getSize().x * 0.5, m_tankTexture.getSize().y * 0.5));
	m_tankSprite.setPosition(m_tankPosition.first, m_tankPosition.second);
}

void Enemy::DoMovement()
{
	if (!gameEngine.MoveTank(this, m_tankDirection, m_enemySpeed)) {
		m_tankDirection = (char)(std::rand() % 4 + 48);
	}
}

void Enemy::SetPoweredUp(bool poweredUp)
{
	m_poweredUp = poweredUp;
}

bool Enemy::GetPoweredUp() const
{
	return m_poweredUp;
}

float Enemy::GetEnemySpeed()
{
	return m_enemySpeed;
}

void Enemy::SetEnemySpeed(float speed)
{
	m_enemySpeed = speed;
}

bool Enemy::IsEnemy() const
{
	return true;
}

void Enemy::FireBullet(std::vector<std::unique_ptr<Bullet>>& m_bulletVec, sf::Time elapsed)
{
	auto tempDirection = GetTankDirection();
	auto tempPos = m_tankSprite.getPosition();
	m_secondCounter += elapsed.asSeconds();
	double flVal = ((rand() % 10) + 10.00) / 10.00;

	if (m_secondCounter >= flVal) {
		switch (tempDirection) {
			case DIR_UP: m_bulletVec.push_back(std::make_unique<Bullet>(std::make_pair(tempPos.x, tempPos.y - 6), tempDirection, this, true)); break;
			case DIR_DOWN: m_bulletVec.push_back(std::make_unique<Bullet>(std::make_pair(tempPos.x, tempPos.y + 6), tempDirection, this, true)); break;
			case DIR_LEFT: m_bulletVec.push_back(std::make_unique<Bullet>(std::make_pair(tempPos.x - 6, tempPos.y), tempDirection, this, true)); break;
			case DIR_RIGHT: m_bulletVec.push_back(std::make_unique<Bullet>(std::make_pair(tempPos.x + 6, tempPos.y), tempDirection, this, true)); break;
		}
		m_secondCounter = 0.f;
	}
}