#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include "Constants.h"

class Player;
class Game;
class GameInput;
class Vampire;

namespace sf
{
	class Clock;
}

class Game : public sf::Drawable
{
public:
	enum class State
	{
		WAITING,
		ACTIVE,
	};

	Game();
	~Game();

	bool initialise();
	void update(float deltaTime);
	void resetLevel();
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	State getState() const { return m_state; }

	void onKeyPressed(sf::Keyboard::Key key);
	void onKeyReleased(sf::Keyboard::Key key);

	Player *getPlayer() const;
	sf::Texture *getPlayerTexture() { return &m_playerTexture; }
	sf::Texture *getVampireTexture() { return &m_vampTexture; }
	sf::Sound *getWeeeSound() { return &m_weeeSound; }
	sf::Sound *getDeathSound() { return &m_deathSound2; }
	sf::Sound *getDeathSound2() { return &m_deathSound2; }
	sf::Sound *getPlayerDeathSound() { return &m_playerDeathSound; }

	void vampireSpawner(float deltaTime);
	void addEnemiesKilled(int n);
	int getEnemiesKilled() { return m_enemiesKilledCount; }

private:
	std::unique_ptr<Player> m_pPlayer;

	std::vector<std::unique_ptr<Vampire>> m_pVampires;

	State m_state;
	std::unique_ptr<sf::Clock> m_pClock;
	std::unique_ptr<GameInput> m_pGameInput;

	float m_vampireCooldown = 0.0f;
	float m_nextVampireCooldown = 2.0f;
	int m_spawnCount = 0;
	int m_enemiesKilledCount = 0;
	int m_bestScore = 0;

	sf::Font m_font;
	sf::Texture m_vampTexture;
	sf::Texture m_playerTexture;
	sf::Texture m_gridTexture;
	sf::SoundBuffer m_weeeSoundBuffer;
	sf::Sound m_weeeSound;
	sf::SoundBuffer m_deathSoundBuffer;
	sf::Sound m_deathSound;
	sf::SoundBuffer m_deathSoundBuffer2;
	sf::Sound m_deathSound2;
	sf::SoundBuffer m_playerDeathBuffer;
	sf::Sound m_playerDeathSound;
	sf::Music m_backgroundMusic;
};
