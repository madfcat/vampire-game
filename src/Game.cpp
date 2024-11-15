#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "ResourceManager.h"
#include "InputHandler.h"
#include "Player.h"
#include "Rectangle.h"
#include "Vampire.h"

Game::Game() : m_state(State::WAITING),
			   m_pClock(std::make_unique<sf::Clock>()),
			   m_pPlayer(std::make_unique<Player>(this)),
			   m_vampireCooldown(2.0f),
			   m_nextVampireCooldown(2.0f)
{
	m_pGameInput = std::make_unique<GameInput>(this, m_pPlayer.get());
}

Game::~Game()
{
}

bool Game::initialise()
{
	/*
		PAY ATTENTION HIVER!
			If you want to load any assets (fonts, textures) please use the pattern shown below
	*/

	if (!m_font.loadFromFile(ResourceManager::getFilePath("Lavigne.ttf")))
	{
		std::cerr << "Unable to load font" << std::endl;
		return false;
	}
	if (!m_vampTexture.loadFromFile(ResourceManager::getFilePath("vampire.png")))
	{
		std::cerr << "Unable to load texture" << std::endl;
		return false;
	}
	if (!m_playerTexture.loadFromFile(ResourceManager::getFilePath("player.png")))
	{
		std::cerr << "Unable to load texture" << std::endl;
		return false;
	}
	if (!m_weeeSoundBuffer.loadFromFile(ResourceManager::getFilePath("weee.ogg")))
	{
		std::cerr << "Unable to load sound" << std::endl;
		return false;
	}
	else
		m_weeeSound.setBuffer(m_weeeSoundBuffer);

	if (!m_deathSoundBuffer.loadFromFile(ResourceManager::getFilePath("death.ogg")))
	{
		std::cerr << "Unable to load sound" << std::endl;
		return false;
	}
	else
		m_deathSound.setBuffer(m_deathSoundBuffer);

	if (!m_deathSoundBuffer2.loadFromFile(ResourceManager::getFilePath("death2.ogg")))
	{
		std::cerr << "Unable to load sound" << std::endl;
		return false;
	}
	else
		m_deathSound2.setBuffer(m_deathSoundBuffer2);

	if (!m_playerDeathBuffer.loadFromFile(ResourceManager::getFilePath("playerdeath.ogg")))
	{
		std::cerr << "Unable to load sound" << std::endl;
		return false;
	}
	else
		m_playerDeathSound.setBuffer(m_playerDeathBuffer);

	if (!m_backgroundMusic.openFromFile(ResourceManager::getFilePath("loop.ogg")))
	{
		std::cerr << "Error: Could not load music!" << std::endl;
		return false;
	}

	m_backgroundMusic.setLoop(true);
	m_backgroundMusic.play();

	resetLevel();
	return true;
}

void Game::resetLevel()
{
	m_pVampires.clear();
	m_spawnCount = 0;
	m_enemiesKilledCount = 0;
	m_pPlayer->initialise();
	m_pClock->restart();
	m_vampireCooldown = 2.0f;
	m_nextVampireCooldown = 2.0f;
}

void Game::update(float deltaTime)
{
	switch (m_state)
	{
	case State::WAITING:
	{
		if (m_pClock->getElapsedTime().asSeconds() >= 3.f)
		{
			m_state = State::ACTIVE;
			m_pClock->restart();
		}
	}
	break;

	case State::ACTIVE:
	{
		m_pGameInput->update(deltaTime);
		m_pPlayer->update(deltaTime);

		vampireSpawner(deltaTime);
		for (auto &temp : m_pVampires)
		{
			temp->update(deltaTime);
		}

		if (m_pPlayer->isDead())
		{
			m_state = State::WAITING;
			resetLevel();
		}
		m_pPlayer->lateUpdate();
	}
	break;
	}

	unsigned long i = 0;
	while (i < m_pVampires.size())
	{
		if (m_pVampires[i]->isKilled())
		{
			std::swap(m_pVampires[i], m_pVampires.back());
			m_pVampires.pop_back();
			continue;
		}
		i++;
	}
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	//  Draw texts.
	if (m_state == State::WAITING)
	{
		sf::Text startText;
		startText.setFont(m_font);
		startText.setString("Game is about to start... Prepare!\n\nUP to move\nLEFT, RIGHT to turn\nHold SHIFT to close path\n\nCatch enemies with the closed trail!");
		startText.setFillColor(sf::Color::White);
		startText.setPosition(80.0f, 80.0f);
		startText.setStyle(sf::Text::Bold);
		target.draw(startText);
	}
	else
	{
		sf::Text timerText;
		timerText.setFont(m_font);
		timerText.setFillColor(sf::Color::White);
		timerText.setStyle(sf::Text::Bold);
		timerText.setString("Time: " + std::to_string((int)m_pClock->getElapsedTime().asSeconds()));
		timerText.setPosition(sf::Vector2f((ScreenWidth - timerText.getLocalBounds().width) * 0.03, 20));
		target.draw(timerText);
		sf::Text enemiesKilledText;
		enemiesKilledText.setFont(m_font);
		enemiesKilledText.setFillColor(sf::Color::White);
		enemiesKilledText.setStyle(sf::Text::Bold);
		enemiesKilledText.setString("Score: " + std::to_string((int)m_enemiesKilledCount) + " / " + std::to_string((int)m_spawnCount));
		enemiesKilledText.setPosition(sf::Vector2f((ScreenWidth - enemiesKilledText.getLocalBounds().width) * 0.03, 60));
		target.draw(enemiesKilledText);
		sf::Text bestScore;
		bestScore.setFont(m_font);
		bestScore.setFillColor(sf::Color::White);
		bestScore.setStyle(sf::Text::Bold);
		bestScore.setString("Best score: " + std::to_string((int)m_bestScore));
		bestScore.setPosition(sf::Vector2f((ScreenWidth - bestScore.getLocalBounds().width) * 0.03, 100));
		target.draw(bestScore);
	}

	// Draw player.
	m_pPlayer->draw(target, states);

	//  Draw world.
	for (auto &temp : m_pVampires)
	{
		temp->draw(target, states);
	}
}

void Game::onKeyPressed(sf::Keyboard::Key key)
{
	m_pGameInput->onKeyPressed(key);
}

void Game::onKeyReleased(sf::Keyboard::Key key)
{
	m_pGameInput->onKeyReleased(key);
}

Player *Game::getPlayer() const
{
	return m_pPlayer.get();
}

void Game::vampireSpawner(float deltaTime)
{
	if (m_vampireCooldown > 0.0f)
	{
		m_vampireCooldown -= deltaTime;
		return;
	}

	float randomXPos = rand() % ScreenWidth;
	float randomYPos = rand() % ScreenHeight;

	float distToRight = (float)ScreenWidth - randomXPos;
	float distToBottom = (float)ScreenHeight - randomYPos;

	float xMinDist = randomXPos < distToRight ? -randomXPos : distToRight;
	float yMinDist = randomYPos < distToBottom ? -randomYPos : distToBottom;

	if (abs(xMinDist) < abs(yMinDist))
		randomXPos += xMinDist;
	else
		randomYPos += yMinDist;

	sf::Vector2f spawnPosition = sf::Vector2f(randomXPos, randomYPos);
	m_pVampires.push_back(std::make_unique<Vampire>(this, spawnPosition));

	m_spawnCount++;
	if (m_spawnCount % 5 == 0)
	{
		m_nextVampireCooldown = std::max(0.3f, m_nextVampireCooldown - 0.1f);
	}
	m_vampireCooldown = m_nextVampireCooldown;

}

void Game::addEnemiesKilled(int n)
{
	m_enemiesKilledCount += n;
	if (m_enemiesKilledCount > m_bestScore)
		m_bestScore = m_enemiesKilledCount;
}