#include "Menu.h"
#include <iostream>
Menu::Menu()
{
	m_titleTexture.loadFromFile("../resources/title.png");
	m_tankTexture.loadFromFile("../resources/tank.png");
	m_gameOverTexture.loadFromFile("../resources/gameOver.png");
	m_stageFlag.loadFromFile("../resources/stageFlag.png");
	m_stageFlagSprite.setTexture(m_stageFlag);
	m_stageFlagSprite.setPosition(sf::Vector2f(733, 570));

	m_gameOverSprite.setTexture(m_gameOverTexture);
	m_gameOverSprite.setPosition(300, 0);

	if (!m_menuFont.loadFromFile("../resources/font.ttf"))
	{
		// error...
		std::cout << "eroare";
	}

	m_scoreDisplay.setFont(m_menuFont);
	m_scoreDisplay.setCharacterSize(30);
	m_scoreDisplay.setStyle(sf::Text::Bold);
	m_scoreDisplay.setFillColor(sf::Color::White);
	m_scoreDisplay.setPosition(200, 500);

	m_menuSprite = CreateSprite();
	m_startText = sf::Text("START", m_menuFont);
	m_onePlayerText = sf::Text("1 PLAYERS", m_menuFont);
	m_twoPlayersText = sf::Text("2 PLAYERS", m_menuFont);
	m_exitText = sf::Text("EXIT", m_menuFont);
	m_pauseText = sf::Text("PAUSED", m_menuFont);
	m_currentStageText = sf::Text("1", m_menuFont, 30);
	m_currentStageText.setPosition(sf::Vector2f(770, 625));
	m_currentStageText.setFillColor(sf::Color(0, 0, 0, 255));

	m_firstPlayerText = sf::Text("IP", m_menuFont, 30);
	m_firstPlayerText.setPosition(sf::Vector2f(730, 365));
	m_firstPlayerText.setFillColor(sf::Color(0, 0, 0, 255));

	m_firstPlayerLivesText = sf::Text("", m_menuFont, 30);
	m_firstPlayerLivesText.setPosition(sf::Vector2f(730, 400));
	m_firstPlayerLivesText.setFillColor(sf::Color(0, 0, 0, 255));

	m_grayBackground.setSize(sf::Vector2f(900, 720));		
	m_grayBackground.setFillColor(sf::Color(127, 127, 127, 255));
	m_grayBackground.setPosition(sf::Vector2f(0, 0));

	m_tankSprite.setTexture(m_tankTexture);
	m_tankSprite.setOrigin(sf::Vector2f(m_tankTexture.getSize().x * 0.5, m_tankTexture.getSize().y * 0.5));
	m_tankSprite.setRotation(90.f);

	//startText.setPosition(387, 350);
	m_onePlayerText.setPosition(337, 350);
	m_twoPlayersText.setPosition(337, 420);
	m_exitText.setPosition(402, 490);
	m_pauseText.setPosition(370, 360);

}

Menu::~Menu()
{

}

sf::Text Menu::GetStartText() {
	return m_startText;
}

sf::Text Menu::GetOnePlayerText()
{
	return m_onePlayerText;
}

sf::Text Menu::GetTwoPlayersText()
{
	return m_twoPlayersText;
}

sf::Sprite Menu::GetMenuSprite()
{
	return m_menuSprite;
}

sf::Sprite Menu::GetTankSprite()
{
	return m_tankSprite;
}

sf::Text Menu::GetExitText()
{
	return m_exitText;
}

sf::Text Menu::GetStageText()
{
	return m_stageText;
}

sf::Text Menu::GetPauseText()
{
	return m_pauseText;
}

sf::Text Menu::GetCurrentStageText()
{
	return m_currentStageText;
}

sf::Text Menu::GetFirstPlayerText()
{
	return m_firstPlayerText;
}

sf::Text Menu::GetFirstPlayerLivesText()
{
	return m_firstPlayerLivesText;
}

void Menu::SetFirstPlayerLivesText(unsigned short lives)
{
	m_firstPlayerLivesText.setString(lives < 10 ? "0" + std::to_string(lives) : std::to_string(lives));
}

bool Menu::GetIsInMenu() const
{
	return m_isInMenu;
}

void Menu::SetIsInMenu(const bool set)
{
	m_isInMenu = set;
}

sf::Sprite Menu::CreateSprite()
{
	sf::Sprite sprite(m_titleTexture);
	sprite.setPosition(240, 60);
	return sprite;
}

int Menu::GetMenuOption() const
{
	return m_menuOption;
}

void Menu::SetMenuOption(const int option)
{
	m_menuOption = option;
}

void Menu::SetStageChooser(const bool stageChooser)
{
	this->m_stageChooser = stageChooser;
}

bool Menu::GetStageChooser() const
{
	return m_stageChooser;
}

void Menu::SetPaused(const bool pause)
{
	m_isPaused = pause;
}

bool Menu::GetPaused() const
{
	return m_isPaused;
}

void Menu::SetScore(sf::Text score)
{
	m_score = score;
}

sf::Text Menu::GetScore()
{
	return m_score;
}

sf::Sprite Menu::GetGameOverSprite() const
{
	return m_gameOverSprite;
}

void Menu::SetGameOverSprite(sf::Sprite gameOverSprite) {
	m_gameOverSprite = gameOverSprite;
}

void Menu::UpdateSprites()
{
	if (GetMenuOption() == 0) {
		m_tankSprite.setPosition(m_onePlayerText.getPosition().x - 35, m_onePlayerText.getPosition().y + 15);
	}
	else if (GetMenuOption() == 1) {
		m_tankSprite.setPosition(m_twoPlayersText.getPosition().x - 35, m_twoPlayersText.getPosition().y + 15);
	}
	else {
		m_tankSprite.setPosition(m_exitText.getPosition().x - 35, m_exitText.getPosition().y + 15);
	}
}

void Menu::DrawStageChangeScene(sf::RenderWindow& window, unsigned int currentStage, sf::Clock& clock, bool& shouldDraw)
{
	sf::Time elapsed = clock.restart();
	m_secondCounter += elapsed.asSeconds();
	std::string stageNumberText = "STAGE " + std::to_string(currentStage + 1);
	m_currentStageText.setString(std::to_string(currentStage + 1));

	m_stageText = sf::Text(stageNumberText, m_menuFont);
	m_stageText.setCharacterSize(35);
	m_stageText.setPosition(330, 320);
	m_stageText.setFillColor(sf::Color::Black);

	if (m_secondCounter <= 3) {
		window.draw(m_grayBackground);
		window.draw(m_stageText);
	}
	else {
		m_secondCounter = 0;
		shouldDraw = false;
	}
}

sf::Font Menu::GetMenuFont()
{
	return m_menuFont;
}

sf::Sprite Menu::GetStageFlag()
{
	return m_stageFlagSprite;
}
