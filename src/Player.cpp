#include "Player.h"
#include "InputHandler.h"
#include "Constants.h"
#include <vector>
#include "Game.h"

Player::Player(Game *pGame) : Rectangle(sf::Vector2f(PlayerWidth, PlayerHeight)),
							  m_pGame(pGame),
							  m_angle(-M_PI / 2)
{
	setOrigin(sf::Vector2f(PlayerWidth / 2, PlayerHeight / 2));
	m_rectangleShape.setPosition(-getSize().x, -getSize().y);
}

bool Player::initialise()
{
	eraseTrail();
	m_sprite.setTexture(*m_pGame->getPlayerTexture());
	sf::Vector2u textureSize = m_sprite.getTexture()->getSize();
	m_sprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
	m_sprite.setScale(getSize().x / textureSize.x, getSize().y / textureSize.y);
	m_sprite.setRotation(0);
	setColor(sf::Color::Green);
	setIsDead(false);
	setPosition(ScreenWidth / 2, ScreenHeight / 2);
	m_sprite.setPosition(getPosition());

	m_isClosed = false;
	m_hasLeftFirstSquare = false;

	return true;
}

void Player::move(InputData inputData, float deltaTime)
{
	m_angle -= RotationSpeed * deltaTime * inputData.m_movingLeft;
	m_angle += RotationSpeed * deltaTime * inputData.m_movingRight;

	if (m_angle < 0)
		m_angle += 2 * M_PI;
	if (m_angle >= 2 * M_PI)
		m_angle -= 2 * M_PI;

	sf::Vector2f direction(std::cos(m_angle), std::sin(m_angle));

	if (inputData.m_movingUp)
	{
		float speed = PlayerSpeed * deltaTime;
		m_sprite.setRotation(m_angle * 180 / M_PI + 90);
		sf::Transformable::move(direction * speed);
		setPosition(std::clamp(getPosition().x, 0.0f, (float)ScreenWidth), getPosition().y);
		setRotation(m_angle * 180 / M_PI);
		m_rectangleShape.setSize(getSize());
		m_rectangleShape.setRotation(getRotation());
	}

	// Update the trail
	if (inputData.m_shift)
	{
		m_trailTimer += deltaTime;
		if (m_trailTimer >= m_trailInterval)
		{
			addTrailSegment();
			m_trailTimer = 0.0f; // Reset timer after adding a segment
		}
	}
	else
	{
		m_trailTimer = 0.0f; // Reset timer if shift is not pressed
		if (m_trail.size() > 0)
			eraseTrail();
	}
}

bool Player::checkPath()
{
	if (isPathClosed())
	{
		m_isClosed = true;
		return true;
	}
	return false;
}

void Player::eraseTrail()
{
	m_trail.erase(m_trail.begin(), m_trail.begin() + m_trail.size());
}

void Player::update(float deltaTime)
{
	m_sprite.setPosition(getPosition());
	checkPath();
}

void Player::lateUpdate()
{
	if (m_isClosed)
	{
		eraseTrail();
		m_isClosed = false;
	}
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	// Draw trail
	for (const auto &segment : m_trail)
	{
		target.draw(segment, states);
	}
	// Draw the player sprite
	Rectangle::draw(target, states);
}

void Player::addTrailSegment()
{

	Rectangle segment(sf::Vector2f(getSize().x, getSize().y), getPosition());
	segment.setOrigin(getOrigin());
	segment.setRotation(getRotation());										 // Set the rotation to match the player’s rotation
	segment.getRectangleShape().setFillColor(sf::Color(100, 100, 255, 150)); // Semi-transparent color for trail
	segment.getRectangleShape().setRotation(getRotation());
	if (m_trail.size() == 0 || segment.getPosition() != m_trail.back().getPosition())
		m_trail.push_back(segment);
}

bool Player::isPathClosed()
{
	// Ensure we have at least 2 segments to start
	if (m_trail.size() < 2)
	{
		return false; // Not enough segments for a loop
	}

	// Get the position of the first trail square (using m_trail[0])
	const sf::Vector2f &firstPosition = m_trail.front().getPosition();
	const sf::Vector2f &lastPosition = m_trail.back().getPosition();

	// Get the global bounds of the first square
	sf::FloatRect firstSquareBounds = const_cast<Rectangle &>(m_trail.front()).getRectangleShape().getGlobalBounds();

	// Check if the player's current position (origin) is inside the first square's bounds
	if (!firstSquareBounds.contains(getPosition() + sf::Vector2f(0, getSize().y / 2)))
	{
		m_hasLeftFirstSquare = true; // Player's origin is outside the first square, so it has left
	}

	bool enteredFirstSquareAgain = false;
	if (m_hasLeftFirstSquare && firstSquareBounds.contains(getPosition() + sf::Vector2f(0, getSize().y / 2)))
	{
		enteredFirstSquareAgain = true;
	}

	// If we've left the first square, check if we've returned to it
	if (enteredFirstSquareAgain)
	{
		m_hasLeftFirstSquare = false;
		return true;
	}

	return false;
}
