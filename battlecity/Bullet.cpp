#include "Engine.h"
#include "Bullet.h"



Bullet::Bullet()
{
}

Bullet::~Bullet()
{
	AnimatedSprite explosion(sf::seconds(0.1), true, false);
	explosion.setPosition(this->m_bulletSprite.getPosition().x, this->m_bulletSprite.getPosition().y);
	explosion.setOrigin(sf::Vector2f(24, 24));
	explosion.play(gameEngine.m_explosionAnim);
	gameEngine.m_explosionsVec.push_back(explosion);
}

Bullet::Bullet(Position position, char direction, Tank* firedBy, bool firedByEnemy) : m_bulletPosition(position), m_bulletDirection(direction), m_firedBy(firedBy), m_firedByEnemy(firedByEnemy)
{
	m_bulletTexture.loadFromFile("../resources/bullet.png");
	m_bulletSprite.setTexture(m_bulletTexture);
	m_bulletTexture.setSmooth(true);
	m_bulletSprite.setOrigin(sf::Vector2f(m_bulletTexture.getSize().x * 0.5, m_bulletTexture.getSize().y * 0.5));
	m_bulletSprite.setPosition(m_bulletPosition.first, m_bulletPosition.second);

	switch (direction) {
	case DIR_UP: m_bulletSprite.setRotation(0.f); break;
	case DIR_DOWN: m_bulletSprite.setRotation(180.f); break;
	case DIR_LEFT: m_bulletSprite.setRotation(-90.f); break;
	case DIR_RIGHT:	m_bulletSprite.setRotation(90.f); break;
	}
}

Tank* Bullet::GetFiredBy() const
{
	return m_firedBy;
}

sf::Texture Bullet::GetTexture()
{
	return m_bulletTexture;
}

Position Bullet::GetPosition() const
{
	return m_bulletPosition;
}

bool Bullet::HandleBullet(std::vector<std::unique_ptr<Bullet>>& bullets, std::vector<std::unique_ptr<WorldEntity>>& worldEntities, std::vector<std::unique_ptr<Enemy>>& enemyTanks, std::vector<PowerUps>& powerUps, std::vector<Position> powerUpSpawnPoints, bool& wallHit, bool& tankHit, bool& solidHit)
{
	float bulletSpeed = 5;
	if (gameEngine.GetStarsCollected() > 0 && !m_firedByEnemy)
		bulletSpeed = 10;

	switch (m_bulletDirection)
	{
		case DIR_UP: this->m_bulletSprite.move(0, -bulletSpeed); break;
		case DIR_DOWN: this->m_bulletSprite.move(0, bulletSpeed); break;
		case DIR_LEFT: this->m_bulletSprite.move(-bulletSpeed, 0); break;
		case DIR_RIGHT: this->m_bulletSprite.move(bulletSpeed, 0); break;
	}

	sf::FloatRect bulletSpriteBounds = this->m_bulletSprite.getGlobalBounds();
	auto bulletItr = std::find_if(bullets.begin(), bullets.end(), [this](std::unique_ptr<Bullet>& element) {return this == element.get(); });

	std::vector<std::unique_ptr<WorldEntity>>::iterator it;

	for (it = worldEntities.begin(); it < worldEntities.end(); it++) {

			if (it->get()->GetType() == entityType::Bush)
				continue;
			if (it->get()->GetType() == entityType::Ice)
				continue;
			if (it->get()->GetType() == entityType::Water)
				continue;

			sf::FloatRect worldEntitySpriteBounds = it->get()->GetSprite().getGlobalBounds();
			
			if (bulletSpriteBounds.intersects(worldEntitySpriteBounds)) {

				bullets.erase(bulletItr);

				if (it->get()->GetType() == entityType::Brick)
				{
					worldEntities.erase(it);
					wallHit = true;
				}
				else if (it->get()->GetType() == entityType::Eagle)
				{
					worldEntities.erase(it);
					gameEngine.SetGameOver(true);
				}
				else if (gameEngine.GetStarsCollected() >= 3 && it->get()->GetType() == entityType::Steel) {
					worldEntities.erase(it);
					solidHit = true;
				}
				else if (it->get()->GetType() == entityType::Steel || it->get()->GetType() == entityType::WorldBound)
				{
					solidHit = true;
				}

				return false;
			}
	}

	if (!m_firedByEnemy)
	for (auto &enemy : enemyTanks) {

		sf::FloatRect enemySpriteBounds = enemy->m_tankSprite.getGlobalBounds();
		auto enemyItr = std::find_if(enemyTanks.begin(), enemyTanks.end(), [&enemy](std::unique_ptr<Enemy>& element) {return enemy == element; });
		
		if (bulletSpriteBounds.intersects(enemySpriteBounds)) {
			tankHit = true;
			bullets.erase(bulletItr);

			if (enemy->GetPoweredUp()) {
				auto generatedPos = rand() % (powerUpSpawnPoints.size() - 1);
				auto randomPowerUp = rand() % 5;
				powerUps.emplace_back(powerUp(randomPowerUp), powerUpSpawnPoints.at(generatedPos).first, powerUpSpawnPoints.at(generatedPos).second);
			}

			enemyTanks.erase(enemyItr);
			gameEngine.SetlocalPlayerKills(gameEngine.GetLocalPlayerKills() + 1);
			gameEngine.SetLocalPlayerScore(gameEngine.GetLocalPlayerScore() + 100);
			return false;
		}
	}

	sf::FloatRect localPlayerBounds = gameEngine.m_localPlayerTank->m_tankSprite.getGlobalBounds();

	if (bulletSpriteBounds.intersects(localPlayerBounds))
	{
		if (!m_firedByEnemy)
			return true;

		if (gameEngine.m_localPlayerLives[0] == 0) {
			gameEngine.SetGameOver(true);
			return true;
		}

		if (gameEngine.m_activePowerUps.at(2) == true)
			return true;

		gameEngine.m_localPlayerLives[0]--;
		gameEngine.SetStarsCollected(0);
		
		// to do: de implementat functionalitatea cand local player-ul este lovit (kill si scazut o viata)

		gameEngine.m_localPlayerSpawnSprite = new AnimatedSprite(sf::seconds(0.25), true, false);
		gameEngine.m_localPlayerSpawnSprite->setPosition(gameEngine.m_localPlayerSpawnPoint.first, gameEngine.m_localPlayerSpawnPoint.second);
		gameEngine.m_localPlayerSpawnSprite->setOrigin(sf::Vector2f(16.5, 15.5));
		gameEngine.m_localPlayerSpawnSprite->play(gameEngine.m_spawnAnim);
		gameEngine.m_localPlayerTank->m_tankSprite.setPosition(1000, 1000);

		bullets.erase(bulletItr);
		tankHit = true;
		return false;
	}

	return true;
}


