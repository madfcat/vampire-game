#include "InputHandler.h"
#include "Player.h"

GameInput::GameInput(Game *pGame, Player *pPlayer) : m_pGame(pGame), m_pPlayer(pPlayer)
{
}

GameInput::~GameInput()
{
}

void GameInput::update(float deltaTime)
{
	if (m_inputData.hasInputs())
	{
		m_pPlayer->move(m_inputData, deltaTime);
	}
}

void GameInput::onKeyPressed(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Up)
	{
		m_inputData.m_movingUp = true;
	}
	else if (key == sf::Keyboard::Down)
	{
		m_inputData.m_movingDown = true;
	}
	// else if (key == sf::Keyboard::Left)
	// {

	//     m_inputData.m_movingRight = false;
	//     m_inputData.m_movingLeft = true;
	// }
	// else if (key == sf::Keyboard::Right)
	// {
	//     m_inputData.m_movingLeft = false;
	//     m_inputData.m_movingRight = true;
	// }
	if (key == sf::Keyboard::Space)
	{
		if (m_inputData.m_spaceReleased)
		{
			m_inputData.m_space = true;
		}
		m_inputData.m_spaceReleased = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && key == sf::Keyboard::Left)
	{
		m_inputData.m_movingRight = false;
		m_inputData.m_movingLeft = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && key == sf::Keyboard::Right)
	{
		m_inputData.m_movingLeft = false;
		m_inputData.m_movingRight = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		m_inputData.m_shift = true;
	}
}

void GameInput::onKeyReleased(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Up)
	{
		m_inputData.m_movingUp = false;
		m_inputData.m_movingLeft = false;
		m_inputData.m_movingRight = false;
	}
	else if (key == sf::Keyboard::Down)
	{
		m_inputData.m_movingDown = false;
	}
	else if (key == sf::Keyboard::Left)
	{
		m_inputData.m_movingLeft = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			m_inputData.m_movingUp = true;
			m_inputData.m_movingRight = true;
		}
	}
	else if (key == sf::Keyboard::Right)
	{
		m_inputData.m_movingRight = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			m_inputData.m_movingUp = true;
			m_inputData.m_movingLeft = true;
		}
	}
	else if (key == sf::Keyboard::Space)
	{
		m_inputData.m_space = false;
		m_inputData.m_spaceReleased = true;
	}
	else if (key == sf::Keyboard::LShift)
	{
		m_inputData.m_shift = false;
	}
}
