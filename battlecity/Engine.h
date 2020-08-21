#pragma once

#include <SFML\Graphics.hpp>
#include "Tank.h"
#include <iostream>
#include <vector>
#include <sstream>
#include "WorldEntity.h"
#include "Enemy.h"
#include "Menu.h"
#include "../Logging/Logger.h"
#include "Bullet.h"
#include "AnimatedSprite.hpp"
#include "Macros.h"
#include "PowerUps.h"

class Engine {

public:
	Engine();
	~Engine();
	void RunGame();
	bool MoveTank(Tank* tankToMove, const char direction, float speed);
	unsigned short TankAlreadyFired(Tank* tankToCheck);
	bool HandleCollision(Tank* tankToCheck, char direction);
	void SetUpWorld(unsigned short stage);
	void DoLocalPlayerMovement();
	void SetlocalPlayerKills(const unsigned int localPlayerKills);
	unsigned int GetLocalPlayerKills() const;
	void SetLocalPlayerScore(const uint64_t localPlayerScore);
	uint64_t GetLocalPlayerScore() const;
	void OnStageStartPresets();
	void SetGameOver(bool gameOver);
	void AdvanceStageSetup();
	void ResetGameLogic();
	void SetUpPUSpawnPoints();
	unsigned short GetStarsCollected() const;
	void SetStarsCollected(unsigned short stars);
	Animation CreateExplosionAnimation();
	Animation CreateSpawnAnimation();

public:
	std::unique_ptr<Tank> m_localPlayerTank;
	Animation m_explosionAnim;
	Animation m_spawnAnim;
	std::vector<AnimatedSprite> m_explosionsVec;
	Position m_localPlayerSpawnPoint;
	AnimatedSprite* m_localPlayerSpawnSprite;
	std::vector<bool> m_activePowerUps = { false, false, false, false, false, false };
	unsigned short m_localPlayerLives[2] = { 2, 2 };
	sf::Sound m_tankMoving;
	sf::Sound m_tankIdleSound;
	bool m_localPlayerTankIsMoving;

private:

	bool m_gameStarted;
	bool m_gameOver;
	unsigned int m_currentStage = 0;
	unsigned int m_stageChosen;
	unsigned int m_localPlayerKills;
	unsigned int m_localPlayerScore = 0;
	unsigned short m_enemiesRespawned = 0;
	unsigned short m_starsCollected = 0;
	float m_secondsElapsed;
	std::vector<std::unique_ptr<WorldEntity>> m_worldEntities;
	std::vector<std::unique_ptr<WorldEntity>> m_iceVec;
	std::vector<std::unique_ptr<WorldEntity>> m_bushVec;
	std::vector<std::unique_ptr<Enemy>> m_enemyTanks;
	std::vector<std::unique_ptr<Bullet>> m_bulletVec;
	std::vector<Position> m_enemySpawnPoints;
	std::vector<Position> m_powerUpSpawnPoints;
	std::vector<PowerUps> m_powerUps;

	sf::SoundBuffer m_tankMovingBuffer;
	sf::SoundBuffer m_bulletBuffer;
	sf::SoundBuffer m_tankIdleBuffer;
	sf::SoundBuffer m_gameOverBuffer;
	sf::SoundBuffer m_wallHitSoundBuffer;
	sf::SoundBuffer m_tankHitSoundBuffer;
	sf::SoundBuffer m_solidHitSoundBuffer;
	sf::SoundBuffer m_powerUpBuffer;
	sf::Sound m_powerUpSound;
	sf::Sound m_solidHitSound;
	sf::Sound m_tankHitSound;
	sf::Sound m_bulletSound;
	sf::Sound m_gameOverSound;
	sf::Sound m_wallHitSound;

	bool m_playedMusic;
	bool m_nextStageScene = true;
	bool m_wallHit = false;
	bool m_tankHit = false;
	bool m_solidHit = false;

	sf::Texture m_enemyLifeTexture;
	sf::Texture m_explosionTextureSheet;
	sf::Texture m_spawnAnimTextureSheet;
	std::vector<sf::Sprite> m_enemyLifeSprites;
	std::vector<AnimatedSprite> m_spawnAnimVec;
};

extern Engine gameEngine;