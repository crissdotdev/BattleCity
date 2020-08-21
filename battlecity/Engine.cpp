#include "Engine.h"
#include "Tank.h"
#include "Macros.h"
#include <fstream>

std::ofstream logFile("log.log", std::ios::app);
Logger logger(std::cout, Logger::Level::Info);

template <class sound>
void movingSounds(sound moving)
{
	gameEngine.m_localPlayerTankIsMoving = moving;
	gameEngine.m_tankIdleSound.stop();

	if (gameEngine.m_tankMoving.getStatus() != sf::Sound::Playing)
	        gameEngine.m_tankMoving.play();
}
Engine::Engine()
{
	if (!m_tankMovingBuffer.loadFromFile("../resources/tankMoving.wav"))
		logger.Logi(Logger::Level::Error, "Nu s-a putut incarca fisierul de muzica.");

	if (!m_bulletBuffer.loadFromFile("../resources/bulletSound.wav"))
		logger.Logi(Logger::Level::Error, "Nu s-a putut incarca fisierul de muzica.");

	if (!m_tankIdleBuffer.loadFromFile("../resources/tankIdle.wav"))
		logger.Logi(Logger::Level::Error, "Nu s-a putut incarca fisierul de muzica.");

	if (!m_gameOverBuffer.loadFromFile("../resources/gameOver.wav"))
		logger.Logi(Logger::Level::Error, "Nu s-a putut incarca fisierul de muzica.");

	if (!m_wallHitSoundBuffer.loadFromFile("../resources/wallHit.wav"))
		logger.Logi(Logger::Level::Error, "Nu s-a putut incarca fisierul de muzica.");

	if (!m_tankHitSoundBuffer.loadFromFile("../resources/tankHit.wav"))
		logger.Logi(Logger::Level::Error, "Nu s-a putut incarca fisierul de muzica.");

	if (!m_solidHitSoundBuffer.loadFromFile("../resources/solidHit.wav"))
		logger.Logi(Logger::Level::Error, "Nu s-a putut incarca fisierul de muzica.");

	if (!m_powerUpBuffer.loadFromFile("../resources/powerUp.wav"))
		logger.Logi(Logger::Level::Error, "Nu s-a putut incarca fisierul de muzica.");

	m_enemyLifeTexture.loadFromFile("../resources/enemyLife.png");
	m_explosionTextureSheet.loadFromFile("../resources/explosion.png");
	m_spawnAnimTextureSheet.loadFromFile("../resources/spawnAnim.png");

	m_tankHitSound.setBuffer(m_tankHitSoundBuffer);
	m_tankHitSound.setVolume(20.5f);

	m_powerUpSound.setBuffer(m_powerUpBuffer);
	m_powerUpSound.setVolume(20.5f);

	m_solidHitSound.setBuffer(m_solidHitSoundBuffer);
	m_solidHitSound.setVolume(10.5f);

	m_explosionAnim = CreateExplosionAnimation();
	m_spawnAnim = CreateSpawnAnimation();

	m_wallHitSound.setBuffer(m_wallHitSoundBuffer);
	m_wallHitSound.setVolume(20.5f);

	m_tankMoving.setBuffer(m_tankMovingBuffer);
	m_tankMoving.setVolume(15.5f);

	m_gameOverSound.setBuffer(m_gameOverBuffer);
	m_gameOverSound.setVolume(25.5f);

	m_bulletSound.setBuffer(m_bulletBuffer);
	m_bulletSound.setVolume(15.5f);

	m_tankIdleSound.setBuffer(m_tankIdleBuffer);
	m_tankIdleSound.setVolume(15.5f);

	OnStageStartPresets();
	SetUpPUSpawnPoints();
}

Engine::~Engine()
{
}

void Engine::RunGame() {

	Menu menu;
	sf::RectangleShape rightSideBg(sf::Vector2f(180, 720));
	rightSideBg.setFillColor(sf::Color(127, 127, 127, 255));
	rightSideBg.setPosition(sf::Vector2f(720, 0));

	sf::Music menuMusic;

	if (!menuMusic.openFromFile("../resources/menumusic.wav"))
		logger.Logi(Logger::Level::Error,"Nu s-a putut incarca fisierul de muzica.");

	menuMusic.setVolume(25.5f);
	menuMusic.play();

	int savedMenuOption = menu.GetMenuOption();

	sf::RenderWindow window(sf::VideoMode(900, 720), "Battle City");
	window.setFramerateLimit(60);
	sf::Clock clock;
	sf::Clock enemyClock;
	sf::Clock animClock;

	while (window.isOpen())
	{

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {

			if(!menu.GetIsInMenu()) {
				m_localPlayerTank->SetTankDirection(DIR_UP);
				logger.Logi(Logger::Level::Debug,"The player is moving upwards");
				movingSounds(true);
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {

			if (!menu.GetIsInMenu()) {
				m_localPlayerTank->SetTankDirection(DIR_DOWN);
				logger.Logi(Logger::Level::Debug,"The player moved downwards");
				movingSounds(true);
			}

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {

			if (!menu.GetIsInMenu()) {
				m_localPlayerTank->SetTankDirection(DIR_LEFT);
				logger.Logi(Logger::Level::Debug,"The player moved to the left");
				movingSounds(true);
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {

			if (!menu.GetIsInMenu()) {
				m_localPlayerTank->SetTankDirection(DIR_RIGHT);
				logger.Logi(Logger::Level::Debug,"The player moved to the right");
				movingSounds(true);
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			logger.Logi(Logger::Level::Info,"Game closed");
			window.close();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			logger.Logi(Logger::Level::Info, "Level skipped, cheater!");
			AdvanceStageSetup();
			OnStageStartPresets();
			m_nextStageScene = true;
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				logger.Logi(Logger::Level::Info,"See you later");
				ResetGameLogic();
				window.close();
			}
			else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Up)
			{
				if (menu.GetIsInMenu()) {
					savedMenuOption = menu.GetMenuOption();
					savedMenuOption--;
					savedMenuOption = std::clamp(savedMenuOption, 0, menu.GetStageChooser() ? 3 : 2);
					menu.SetMenuOption(savedMenuOption);
				}
				else
				{
					m_localPlayerTankIsMoving = false;
					m_tankMoving.stop();
					m_tankIdleSound.setLoop(true);
					m_tankIdleSound.play();
				}
			}
			else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down)
			{
				if (menu.GetIsInMenu()) {
					savedMenuOption = menu.GetMenuOption();
					savedMenuOption++;
					savedMenuOption = std::clamp(savedMenuOption, 0, menu.GetStageChooser() ? 3 : 2);
					menu.SetMenuOption(savedMenuOption);
				}
				else
				{
					m_localPlayerTankIsMoving = false;
					m_tankMoving.stop();
					m_tankIdleSound.setLoop(true);
					m_tankIdleSound.play();
				}
			}
			else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Left)
			{
				if (!menu.GetIsInMenu()) {
					m_localPlayerTankIsMoving = false;
					m_tankMoving.stop();
					m_tankIdleSound.setLoop(true);
					m_tankIdleSound.play();
				}
			}
			else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Right)
			{
				if (!menu.GetIsInMenu()) {
					m_localPlayerTankIsMoving = false;
					m_tankMoving.stop();
					m_tankIdleSound.setLoop(true);
					m_tankIdleSound.play();
				}
			}
			else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter) {

				if (menu.GetIsInMenu() && menu.GetMenuOption() == 0) {

					ResetGameLogic();
					OnStageStartPresets();
					SetUpWorld(0);
					m_tankIdleSound.play();
					menu.SetIsInMenu(false);
				}
				else if (menu.GetIsInMenu() && menu.GetMenuOption() == 2) {
					logger.Logi(Logger::Level::Error, "Really, you don't want to play our game? :'(");
					window.close();
				}


			}
			else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::P) {
				
				menu.GetPaused() ? menu.SetPaused(false) : menu.SetPaused(true);
				if (menu.GetPaused() == false)
					logger.Logi(Logger::Level::Info, "Game resumed");
				else
					logger.Logi(Logger::Level::Info, "Game paused");
			}
			else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {

				if (menu.GetIsInMenu())
					continue;

				if (TankAlreadyFired(m_localPlayerTank.get()) > 0 && m_starsCollected == 0)
					continue;

				if (TankAlreadyFired(m_localPlayerTank.get()) > 1 && m_starsCollected > 1)
					continue;

				if ((m_localPlayerTank->m_tankSprite.getPosition().x > 900 ||
					m_localPlayerTank->m_tankSprite.getPosition().x < 0) ||
					(m_localPlayerTank->m_tankSprite.getPosition().y > 720 ||
					m_localPlayerTank->m_tankSprite.getPosition().y < 0))
					continue;

				logger.Logi(Logger::Level::Debug, "Fire");
				auto tempDirection = m_localPlayerTank->GetTankDirection();
				auto tempPos = m_localPlayerTank->m_tankSprite.getPosition();
				
				switch (tempDirection) {
					case DIR_UP: m_bulletVec.push_back(std::make_unique<Bullet>(std::make_pair(tempPos.x, tempPos.y - 6), tempDirection, m_localPlayerTank.get(), false)); break;
					case DIR_DOWN: m_bulletVec.push_back(std::make_unique<Bullet>(std::make_pair(tempPos.x, tempPos.y + 6), tempDirection, m_localPlayerTank.get(), false)); break;
					case DIR_LEFT: m_bulletVec.push_back(std::make_unique<Bullet>(std::make_pair(tempPos.x - 6, tempPos.y), tempDirection, m_localPlayerTank.get(), false)); break;
					case DIR_RIGHT: m_bulletVec.push_back(std::make_unique<Bullet>(std::make_pair(tempPos.x + 6, tempPos.y), tempDirection, m_localPlayerTank.get(), false)); break;
				}

				m_bulletSound.play();
			}

		}

		//advance to the next stage if player has killed 20 enemy tanks
		if (m_localPlayerKills == 20)
		{
			if (m_currentStage != 21) 
			{
				AdvanceStageSetup();
				OnStageStartPresets();
			}
			else
			{
				m_gameOver = true;
			}
			m_nextStageScene = true;
			menuMusic.play();
		}

		window.clear();

		//draw stuff

		if (menu.GetIsInMenu()) {
			menu.UpdateSprites();
			window.draw(menu.GetMenuSprite());
			window.draw(menu.GetTankSprite());
			window.draw(menu.GetOnePlayerText());
			window.draw(menu.GetTwoPlayersText());
			window.draw(menu.GetExitText());
		}
		else if (menu.GetPaused()) {
			window.draw(menu.GetPauseText());
		}
		else if (m_gameOver)
		{
			menu.m_scoreDisplay.setString("You score is: " + std::to_string(m_localPlayerScore));
			window.draw(menu.m_gameOverSprite);
			window.draw(menu.m_scoreDisplay);
			m_tankIdleSound.stop();
			m_tankMoving.stop();

			if (menu.m_gameOverSprite.getPosition().y != 200) {
				menu.m_gameOverSprite.move(0, 1);
			}
			else {
				ResetGameLogic();
				menu.m_gameOverSprite.setPosition(300, 0);
				menu.SetIsInMenu(true);
			}

			if (m_gameOverSound.getStatus() != sf::Sound::Playing && m_playedMusic == false)
			{
				m_gameOverSound.play();
				m_playedMusic = true;
			}
			
		}
		else if (m_nextStageScene) {
			menu.DrawStageChangeScene(window, m_currentStage, clock, m_nextStageScene);
			float secondCounter = 0.f;
			m_tankMoving.stop();
			m_bulletSound.stop();	
		}
		else if (!menu.GetIsInMenu()) {

			clock.restart();
			sf::Time frameTime = animClock.restart();
			sf::Time elapsed = enemyClock.restart();


			// powerUps
			{
				//grenade
				if (m_activePowerUps.at(0)) {
					m_enemyTanks.clear();
					logger.Logi(Logger::Level::Info, "All enemies disappeared");
					m_activePowerUps.at(0) = false;
				}
				
				// tank - extra life
				if (m_activePowerUps.at(5)) {
					m_localPlayerLives[0]++;
					m_localPlayerLives[1]++;
					logger.Logi(Logger::Level::Info, "You received an extra life");
					m_activePowerUps.at(5) = false;
				}

				// shield
				if (m_activePowerUps.at(2)) {
					m_secondsElapsed += frameTime.asSeconds();

					if (m_secondsElapsed >= 10) {
						m_secondsElapsed = 0;
						m_activePowerUps.at(2) = false;
						logger.Logi(Logger::Level::Info, "You are no longer immune!");
					}
					
				}

				
			}

			//draw ice first - tank should be over ice so we have to draw ice first
			for (auto& entity : m_iceVec) {
				window.draw(entity->GetSprite());
			}

			//do movement and draw local player
			DoLocalPlayerMovement();
			window.draw(m_localPlayerTank->m_tankSprite);

			//do movement and draw enemies
			for (auto &enemyTank : m_enemyTanks) {
			
				// timer power up, enemies should freeze
				if (m_activePowerUps.at(1)) {
					m_secondsElapsed += frameTime.asSeconds();
					if (m_secondsElapsed >= 10)
					{
						m_secondsElapsed = 0;
						m_activePowerUps.at(1) = false;
						logger.Logi(Logger::Level::Info, "Your power up expired, enemies are no longer frozen");
					}
				}
				else {
					enemyTank->DoMovement();
				}

				if (TankAlreadyFired(enemyTank.get()) == 0 && !m_activePowerUps.at(1)) {
					enemyTank->FireBullet(m_bulletVec, elapsed);
				}

				window.draw(enemyTank->m_tankSprite);
			}

			//draw world entities
			for (auto &entity : m_worldEntities) {
				window.draw(entity->GetSprite());
			}

			//bullet logic and draw bullets
			for (auto& bullets : m_bulletVec) {

				if (!bullets.get()->HandleBullet(m_bulletVec, m_worldEntities, m_enemyTanks, m_powerUps, m_powerUpSpawnPoints, m_wallHit, m_tankHit, m_solidHit))
				{
					break;
				}
				if (m_wallHit == true)
				{
					m_wallHitSound.play();
					m_wallHit = false;
				}
				if (m_solidHit == true)
				{
					m_solidHitSound.play();
					m_solidHit = false;
				}
				if (m_tankHit == true)
				{
					m_tankHitSound.play();
					m_tankHit = false;
				}
				window.draw(bullets.get()->m_bulletSprite);
			}

			for (auto& explosion : m_explosionsVec) {
				explosion.update(frameTime);
				window.draw(explosion);
			}

			m_explosionsVec.erase(std::remove_if(m_explosionsVec.begin(), m_explosionsVec.end(), [](AnimatedSprite sprite) { return sprite.isPlaying() == false; }), m_explosionsVec.end());

			//draw bush - bullet should be under bush so we have to draw bush first
			for (auto& entity : m_bushVec) {
				window.draw(entity->GetSprite());
			}

			//we should always have 3 enemies on the map
			if (m_enemyTanks.size() + m_spawnAnimVec.size() < 3) {

				auto generatedPos = m_enemySpawnPoints.at(rand() % m_enemySpawnPoints.size());
				bool okToSpawn = false;

				/*sf::RectangleShape dummy;
				dummy.setSize(sf::Vector2f(27, 30));
				dummy.setPosition(generatedPos.first, generatedPos.second);

				for (auto& enemy : m_enemyTanks) {
					if (!dummy.getGlobalBounds().intersects(enemy->m_tankSprite.getGlobalBounds()))
						okToSpawn = true;
				}

				if (okToSpawn) {*/
				AnimatedSprite spawn(sf::seconds(0.25), true, false);
				spawn.setPosition(generatedPos.first, generatedPos.second);
				spawn.setOrigin(sf::Vector2f(16.5, 15.5));
				spawn.play(m_spawnAnim);
				m_spawnAnimVec.push_back(spawn);

				if (!m_enemyLifeSprites.empty())
					m_enemyLifeSprites.pop_back();
				//}

			}

			for (auto& spawnAnim : m_spawnAnimVec) {

				spawnAnim.update(frameTime);
				window.draw(spawnAnim);

				if (!spawnAnim.isPlaying()) {
					m_enemiesRespawned++;

					if (m_enemiesRespawned == 1 || m_enemiesRespawned == 8 || m_enemiesRespawned == 15) {
						m_enemyTanks.push_back(std::make_unique<Enemy>(spawnAnim.getPosition().x, spawnAnim.getPosition().y, true));
					}
					else
					{
						m_enemyTanks.push_back(std::make_unique<Enemy>(spawnAnim.getPosition().x, spawnAnim.getPosition().y, false));
					}
								
				}

			}

			m_spawnAnimVec.erase(std::remove_if(m_spawnAnimVec.begin(), m_spawnAnimVec.end(), [](AnimatedSprite sprite) { return sprite.isPlaying() == false; }), m_spawnAnimVec.end());

			for (auto& powerUp : m_powerUps) {
				window.draw(powerUp.GetSprite());
			}

			if (m_localPlayerSpawnSprite != nullptr) {
				m_localPlayerSpawnSprite->update(frameTime);
				window.draw(*m_localPlayerSpawnSprite);

				if (!m_localPlayerSpawnSprite->isPlaying()) {
					m_localPlayerTank->m_tankSprite.setPosition(gameEngine.m_localPlayerSpawnPoint.first, gameEngine.m_localPlayerSpawnPoint.second);
					delete m_localPlayerSpawnSprite;
					m_localPlayerSpawnSprite = nullptr;
				}
			}

			window.draw(rightSideBg);

			for (auto enemyLife : m_enemyLifeSprites) {
				window.draw(enemyLife);
			}

			window.draw(menu.GetStageFlag());
			window.draw(menu.GetCurrentStageText());
			window.draw(menu.GetFirstPlayerText());
			menu.SetFirstPlayerLivesText(m_localPlayerLives[0]);
			window.draw(menu.GetFirstPlayerLivesText());

		}
		
		window.display();
	}
}

bool Engine::MoveTank(Tank* tankToMove, const char direction, float speed)
{
	sf::FloatRect intersection;
	
	switch (direction) {
	case DIR_UP:

		if (tankToMove->m_tankSprite.getRotation() != 0)
			tankToMove->m_tankSprite.setRotation(0.f);

		if (HandleCollision(tankToMove, tankToMove->GetTankDirection()))
			tankToMove->m_tankSprite.move(0, -speed);
		else
			return false;

		break;

	case DIR_DOWN:

		if (tankToMove->m_tankSprite.getRotation() != 180)
			tankToMove->m_tankSprite.setRotation(180.f);

		if (HandleCollision(tankToMove, tankToMove->GetTankDirection()))
			tankToMove->m_tankSprite.move(0, speed);
		else
			return false;
		
		break;

	case DIR_LEFT:

		if (tankToMove->m_tankSprite.getRotation() != -90)
			tankToMove->m_tankSprite.setRotation(-90.f);

		if (HandleCollision(tankToMove, tankToMove->GetTankDirection()))
			tankToMove->m_tankSprite.move(-speed, 0);
		else
			return false;

		break;

	case DIR_RIGHT:

		if (tankToMove->m_tankSprite.getRotation() != 90)
			tankToMove->m_tankSprite.setRotation(90.f);

		if (HandleCollision(tankToMove, tankToMove->GetTankDirection()))
			tankToMove->m_tankSprite.move(speed, 0);
		else
			return false;

		break;
	}

	// daca s-a executat miscarea, returnam true
	return true;
}

unsigned short Engine::TankAlreadyFired(Tank* tankToCheck)
{
	unsigned short numberOfBullets = 0;

	for (auto& bullet : m_bulletVec) {
		if (bullet->GetFiredBy() == tankToCheck) {
			numberOfBullets++;
		}
	}

	return numberOfBullets;
}

bool Engine::HandleCollision(Tank* tankToCheck, char direction)
{
	auto dummySprite = tankToCheck->m_tankSprite;
	auto futurePosition = tankToCheck->m_tankSprite.getPosition();

	switch (direction) {
	case DIR_UP: futurePosition.y -= tankToCheck->GetTankSpeed(); break;
	case DIR_DOWN: futurePosition.y += tankToCheck->GetTankSpeed(); break;
	case DIR_LEFT: futurePosition.x -= tankToCheck->GetTankSpeed(); break;
	case DIR_RIGHT: futurePosition.x += tankToCheck->GetTankSpeed(); break;
	}
	
	dummySprite.setPosition(futurePosition);

	sf::FloatRect bounds = dummySprite.getGlobalBounds();

	for (auto& entity : m_worldEntities) {

		if (entity->GetType() == entityType::Bush)
			continue;
		if (entity->GetType() == entityType::Ice)
			continue;

		sf::FloatRect secondSpriteBounds = entity->GetSprite().getGlobalBounds();

		if (bounds.intersects(secondSpriteBounds)) {
			return false;
		}
	}

	for (auto& enemyTank : m_enemyTanks) {

		if (tankToCheck == enemyTank.get())
			continue;

		sf::FloatRect secondSpriteBounds = enemyTank->m_tankSprite.getGlobalBounds();

		if (bounds.intersects(secondSpriteBounds)) {
			return false;
		}
	}

	//for (auto& spawnAnimation : spawnAnimVec) {
	//	sf::FloatRect secondSpriteBounds = spawnAnimation.getGlobalBounds();

	//	if (bounds.intersects(secondSpriteBounds)) {
	//		return false;
	//	}
	//}

	if (tankToCheck == m_localPlayerTank.get()) {
		std::vector<PowerUps>::iterator it;
		for (it = m_powerUps.begin(); it < m_powerUps.end(); it++) {
		
			sf::FloatRect secondSpriteBounds = it->GetSprite().getGlobalBounds();

			if (bounds.intersects(secondSpriteBounds)) {
				auto type = it->GetType();
				m_powerUpSound.play();
				logger.Logi(Logger::Level::Info, "You picked up a power up [", type, "]");

				if (it->GetType() == powerUp::StarPU) {
					m_starsCollected++;
					logger.Logi(Logger::Level::Info, "You picked up a star, now you have ", m_starsCollected);
				}

				m_activePowerUps.at(type) = true;
				m_powerUps.erase(it);
				break;
			}
		}
	}

	if (tankToCheck != m_localPlayerTank.get()) {
		if (bounds.intersects(m_localPlayerTank->m_tankSprite.getGlobalBounds())) {
			return false;
		}
	}

	return true;
}

void Engine::DoLocalPlayerMovement()
{
	if (m_localPlayerTankIsMoving) 
	{
		gameEngine.MoveTank(m_localPlayerTank.get(), m_localPlayerTank->GetTankDirection(), m_localPlayerTank->GetTankSpeed());
	}
}

void Engine::SetlocalPlayerKills(const unsigned int localPlayerKills)
{
	m_localPlayerKills = localPlayerKills;
}

unsigned int Engine::GetLocalPlayerKills() const
{
	return m_localPlayerKills;
}

void Engine::SetLocalPlayerScore(const uint64_t localPlayerScore)
{
	m_localPlayerScore = localPlayerScore;
}

uint64_t Engine::GetLocalPlayerScore() const
{
	return m_localPlayerScore;
}

void Engine::OnStageStartPresets()
{
	unsigned short x = 630;
	unsigned short y = 20;

	m_enemyLifeSprites.clear();

	for (int i = 0; i < 20; i++) {
		sf::Sprite enemyLifeSprite;
		enemyLifeSprite.setTexture(m_enemyLifeTexture);
		if (i % 2 == 0) {
			x = 730;
			y += 30;
		}
		else {
			x = 760;
		}
			
		enemyLifeSprite.setPosition(x, y);
		m_enemyLifeSprites.push_back(enemyLifeSprite);
	}

	m_gameOver = false;
	m_localPlayerKills = 0;
	m_localPlayerTankIsMoving = false;
	m_playedMusic = false;
}

void Engine::AdvanceStageSetup()
{
	m_currentStage += 1;
	if (m_currentStage == 10 || m_currentStage == 20 || m_currentStage == 30)
	{
		m_localPlayerLives[0]++;
		logger.Logi(Logger::Level::Info, "Felicitari ai primit o viata in plus");
	}
	m_worldEntities.clear();
	m_bushVec.clear();
	m_iceVec.clear();
	m_enemyTanks.clear();
	m_bulletVec.clear();

	m_tankIdleSound.play();
	m_tankMoving.stop();
	m_bulletSound.stop();
	m_gameOver = false;
	m_localPlayerKills = 0;

	m_activePowerUps = { false, false, false, false, false, false };

	SetUpWorld(m_currentStage);
}

void Engine::ResetGameLogic()
{
	m_currentStage = 0;
	m_worldEntities.clear();
	m_bushVec.clear();
	m_iceVec.clear();
	m_enemyTanks.clear();
	m_bulletVec.clear();
	m_gameOver = false;
	m_localPlayerKills = 0;
	m_localPlayerTankIsMoving = false;
	m_localPlayerLives[0] = 2;
	m_playedMusic = false;
	m_localPlayerScore = 0;
	m_enemiesRespawned = 0;
	m_explosionsVec.clear();
	m_spawnAnimVec.clear();
	
	m_tankIdleSound.stop();
	m_tankMoving.stop();
	m_bulletSound.stop();

}

void Engine::SetUpPUSpawnPoints()
{
	m_powerUpSpawnPoints.push_back(std::make_pair(150, 150));
	m_powerUpSpawnPoints.push_back(std::make_pair(474, 194));
	m_powerUpSpawnPoints.push_back(std::make_pair(140, 370));
	m_powerUpSpawnPoints.push_back(std::make_pair(69, 463));
	m_powerUpSpawnPoints.push_back(std::make_pair(153, 555));
	m_powerUpSpawnPoints.push_back(std::make_pair(308, 571));
	m_powerUpSpawnPoints.push_back(std::make_pair(436, 464));
	m_powerUpSpawnPoints.push_back(std::make_pair(588, 378));
	m_powerUpSpawnPoints.push_back(std::make_pair(630, 557));
	m_powerUpSpawnPoints.push_back(std::make_pair(548, 630));
	m_powerUpSpawnPoints.push_back(std::make_pair(459, 548));
	m_powerUpSpawnPoints.push_back(std::make_pair(572, 269));
	m_powerUpSpawnPoints.push_back(std::make_pair(598, 74));
	m_powerUpSpawnPoints.push_back(std::make_pair(359, 214));
	m_powerUpSpawnPoints.push_back(std::make_pair(408, 294));	
	m_powerUpSpawnPoints.push_back(std::make_pair(305, 294));
}

unsigned short Engine::GetStarsCollected() const
{
	return m_starsCollected;
}

void Engine::SetStarsCollected(unsigned short stars)
{
	m_starsCollected = stars;
}

Animation Engine::CreateExplosionAnimation()
{
	Animation explosionAnimation;
	explosionAnimation.setSpriteSheet(m_explosionTextureSheet);
	
	for (int i = 0; i < 5; i++)
		explosionAnimation.addFrame(sf::IntRect(0, i * 48, 48, 48));

	return explosionAnimation;
}

Animation Engine::CreateSpawnAnimation()
{
	Animation spawnAnimation;
	spawnAnimation.setSpriteSheet(m_spawnAnimTextureSheet);

	for (int i = 0; i < 10; i++)
		spawnAnimation.addFrame(sf::IntRect(0, i * 31, 33, 31));

	return spawnAnimation;
}

void Engine::SetGameOver(bool gameOver)
{
	m_gameOver = gameOver;
}


void Engine::SetUpWorld(unsigned short stage)
{
	logger.Logi(Logger::Level::Info, "Game Started");
	unsigned short x = 0;
	unsigned short y = 0;
	float worldEntitySize = 48;
	std::vector<std::string> fileNames = { "../stages/stage1.txt", "../stages/stage2.txt", "../stages/stage3.txt", "../stages/stage4.txt", "../stages/stage5.txt"
	, "../stages/stage6.txt", "../stages/stage7.txt", "../stages/stage8.txt", "../stages/stage9.txt", "../stages/stage10.txt", "../stages/stage11.txt", "../stages/stage12.txt"
	, "../stages/stage13.txt", "../stages/stage14.txt", "../stages/stage15.txt", "../stages/stage16.txt", "../stages/stage17.txt", "../stages/stage18.txt", 
		"../stages/stage19.txt", "../stages/stage20.txt" , "../stages/stage21.txt", "../stages/stage22.txt", "../stages/stage23.txt" };
	std::string inputFromFile;
	std::ifstream file(fileNames.at(stage));

	while (std::getline(file, inputFromFile)) {
		x = 0;
		for (auto chr : inputFromFile) {

			switch (chr) {
				case 'w': {

					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::WorldBound, x * worldEntitySize + worldEntitySize / 2, y * worldEntitySize + worldEntitySize / 2));
					break;
				}
				case 'b': {
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Brick, x * worldEntitySize, y * worldEntitySize));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Brick, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Brick, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Brick, x * worldEntitySize, y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Brick, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Brick, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Brick, x * worldEntitySize, y * worldEntitySize + worldEntitySize / 3));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Brick, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize + worldEntitySize / 3));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Brick, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize + worldEntitySize / 3));
					break;
				}
				case 's': {
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Steel, x * worldEntitySize, y * worldEntitySize));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Steel, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Steel, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Steel, x * worldEntitySize, y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Steel, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Steel, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Steel, x * worldEntitySize, y * worldEntitySize + worldEntitySize / 3));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Steel, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize + worldEntitySize / 3));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Steel, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize + worldEntitySize / 3));
					break;
				}
				case 'e': {
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Eagle, x * worldEntitySize + worldEntitySize / 2, y * worldEntitySize + worldEntitySize / 2));
					break;
				}
				case ' ': {

					break;
				}
				case 'g': {
					m_bushVec.push_back(std::make_unique<WorldEntity>(entityType::Bush, x * worldEntitySize, y * worldEntitySize));
					m_bushVec.push_back(std::make_unique<WorldEntity>(entityType::Bush, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize));
					m_bushVec.push_back(std::make_unique<WorldEntity>(entityType::Bush, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize));
					m_bushVec.push_back(std::make_unique<WorldEntity>(entityType::Bush, x * worldEntitySize, y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_bushVec.push_back(std::make_unique<WorldEntity>(entityType::Bush, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_bushVec.push_back(std::make_unique<WorldEntity>(entityType::Bush, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_bushVec.push_back(std::make_unique<WorldEntity>(entityType::Bush, x * worldEntitySize, y * worldEntitySize + worldEntitySize / 3));
					m_bushVec.push_back(std::make_unique<WorldEntity>(entityType::Bush, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize + worldEntitySize / 3));
					m_bushVec.push_back(std::make_unique<WorldEntity>(entityType::Bush, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize + worldEntitySize / 3));
					break;
				}
				case 'i': {
					m_iceVec.push_back(std::make_unique<WorldEntity>(entityType::Ice, x * worldEntitySize, y * worldEntitySize));
					m_iceVec.push_back(std::make_unique<WorldEntity>(entityType::Ice, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize));
					m_iceVec.push_back(std::make_unique<WorldEntity>(entityType::Ice, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize));
					m_iceVec.push_back(std::make_unique<WorldEntity>(entityType::Ice, x * worldEntitySize, y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_iceVec.push_back(std::make_unique<WorldEntity>(entityType::Ice, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_iceVec.push_back(std::make_unique<WorldEntity>(entityType::Ice, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_iceVec.push_back(std::make_unique<WorldEntity>(entityType::Ice, x * worldEntitySize, y * worldEntitySize + worldEntitySize / 3));
					m_iceVec.push_back(std::make_unique<WorldEntity>(entityType::Ice, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize + worldEntitySize / 3));
					m_iceVec.push_back(std::make_unique<WorldEntity>(entityType::Ice, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize + worldEntitySize / 3));
					break;
				}
				case 'a': {
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Water, x * worldEntitySize, y * worldEntitySize));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Water, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Water, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Water, x * worldEntitySize, y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Water, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Water, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize + 2 * (worldEntitySize / 3)));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Water, x * worldEntitySize, y * worldEntitySize + worldEntitySize / 3));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Water, x * worldEntitySize + worldEntitySize / 3, y * worldEntitySize + worldEntitySize / 3));
					m_worldEntities.push_back(std::make_unique<WorldEntity>(entityType::Water, x * worldEntitySize + 2 * (worldEntitySize / 3), y * worldEntitySize + worldEntitySize / 3));
					break;
				}
				case 'h': {
					m_enemyTanks.push_back(std::make_unique<Enemy>(x * worldEntitySize + worldEntitySize / 2, y * worldEntitySize + worldEntitySize / 2, false));
					m_enemySpawnPoints.push_back(std::make_pair(x * worldEntitySize + worldEntitySize / 2, y * worldEntitySize + worldEntitySize / 2));
					break;
				}
				case 'p': {
					m_localPlayerTank = std::make_unique<Tank>(x * worldEntitySize + worldEntitySize / 2, y * worldEntitySize + worldEntitySize / 2);
					m_localPlayerSpawnPoint.first = x * worldEntitySize + worldEntitySize / 2;
					m_localPlayerSpawnPoint.second = y * worldEntitySize + worldEntitySize / 2;
				}
			}

			x++;
		}
		y++;
	}
	logger.Logi(Logger::Level::Info, m_worldEntities.size(), "elements were loaded in this stage ");

}


