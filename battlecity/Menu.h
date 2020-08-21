#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <algorithm>

class Menu
{
public:
	Menu();
	~Menu();

	bool GetIsInMenu() const;
	void SetIsInMenu(const bool set);
	int GetMenuOption() const;
	void SetMenuOption(const int option);
	void SetStageChooser(const bool stageChooser);
	bool GetStageChooser() const;
	void SetPaused(const bool pause);
	bool GetPaused() const;
	void SetScore(sf::Text score);
	void UpdateSprites();
	void DrawStageChangeScene(sf::RenderWindow& window, unsigned int currentStage, sf::Clock& clock, bool& shouldDraw);
	void SetGameOverSprite(sf::Sprite gameOverSprite);
	void SetFirstPlayerLivesText(unsigned short lives);

	sf::Text GetScore();
	sf::Sprite CreateSprite();
	sf::Sprite GetGameOverSprite() const;

	sf::Text GetStartText();
	sf::Text GetOnePlayerText();
	sf::Text GetTwoPlayersText();
	sf::Sprite GetMenuSprite();
	sf::Sprite GetTankSprite();
	sf::Text GetExitText();
	sf::Text GetStageText();
	sf::Text GetPauseText();
	sf::Text GetCurrentStageText();
	sf::Text GetFirstPlayerText();
	sf::Text GetFirstPlayerLivesText();
	sf::Font GetMenuFont();
	sf::Sprite GetStageFlag();

public:
	sf::Texture m_gameOverTexture;
	sf::Sprite m_gameOverSprite;
	sf::Text m_scoreDisplay;

private:

	bool m_isInMenu = true;
	bool m_isPaused = false;
	sf::Texture m_titleTexture;
	int m_menuOption = 0;
	bool m_stageChooser = false;
	float m_secondCounter = 0.f;

	sf::Font m_menuFont;
	sf::Text m_startText;
	sf::Text m_onePlayerText;
	sf::Text m_twoPlayersText;

	sf::Text m_exitText;
	sf::Text m_pauseText;
	sf::Text m_stageText;
	sf::Text m_currentStageText;
	sf::Text m_firstPlayerText;
	sf::Text m_firstPlayerLivesText;

	sf::Sprite m_menuSprite;
	sf::Texture m_tankTexture;
	sf::Sprite m_tankSprite;
	sf::Text m_gameOver;
	sf::Text m_score;
	sf::RectangleShape m_grayBackground;
	sf::Texture m_stageFlag;
	sf::Sprite m_stageFlagSprite;

};
