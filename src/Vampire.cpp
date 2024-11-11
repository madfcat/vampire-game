#include "Vampire.h"
#include "Constants.h"
#include "Game.h"
#include "Player.h"
#include "MathUtils.h"

Vampire::Vampire(Game *game, sf::Vector2f position) : Rectangle(sf::Vector2f(VampireWidth, VampireHeight)),
													  m_pGame(game)
{
	setPosition(position);
	setColor(sf::Color::Blue);
	setOrigin(sf::Vector2f(VampireWidth / 2, VampireHeight / 2));
	setIsKilled(false);

	m_sprite.setTexture(*m_pGame->getVampireTexture());
	sf::Vector2u textureSize = (*m_pGame->getVampireTexture()).getSize();
	m_sprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
	m_sprite.setScale(VampireWidth / textureSize.x, VampireWidth / textureSize.y);
}

void Vampire::update(float deltaTime)
{
	int randomChoice = rand() % 2;

	if (m_isKilled)
		return;

	Player *pPlayer = m_pGame->getPlayer();

	if (isEnemyBetweenTrailPoints(pPlayer))
	{
		m_pGame->addEnemiesKilled(1);
		setIsKilled(true);
		if (randomChoice == 0)
			m_pGame->getDeathSound()->play();
		else
			m_pGame->getDeathSound2()->play();
		return;
	}

	if (collidesWith(pPlayer))
	{
		pPlayer->setIsDead(true);
		m_pGame->getPlayerDeathSound()->play();
	}

	sf::Vector2f playerCenter = pPlayer->getCenter();
	sf::Vector2f direction = VecNormalized(playerCenter - getCenter());
	direction *= VampireSpeed * deltaTime;
	sf::Transformable::move(direction);
	m_rectangleShape.setPosition(-getSize().x, -getSize().y);
	m_sprite.setPosition(getPosition());
}

bool isPointInPolygon(const sf::Vector2f &point, const std::vector<sf::Vector2f> &polygon)
{
	int n = polygon.size();
	bool inside = false;

	for (int i = 0, j = n - 1; i < n; j = i++)
	{
		const sf::Vector2f &p1 = polygon[i];
		const sf::Vector2f &p2 = polygon[j];

		// Check if the point is between the y-coordinates of the segment
		if ((point.y > p1.y && point.y <= p2.y) || (point.y > p2.y && point.y <= p1.y))
		{
			if (point.x < (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x)
			{
				inside = !inside;
			}
		}
	}

	return inside;
}

bool Vampire::isEnemyBetweenTrailPoints(Player *pPlayer)
{
	auto &trail = pPlayer->getTrail();
	if (trail.size() < 2)
		return false;

	if (pPlayer->getIsClosed())
	{
		const sf::Vector2f &enemyPosition = getPosition(); // Get the position of the enemy (Vampire)

		// Create a polygon from the trail points (you can decide on how many points to use, e.g., every 4 points)
		std::vector<sf::Vector2f> polygon;
		for (size_t i = 0; i < trail.size(); ++i)
		{
			polygon.push_back(trail[i].getPosition());
		}

		// Check if the enemy is inside the polygon formed by the trail points
		if (isPointInPolygon(enemyPosition, polygon))
		{
			return true; // Enemy is inside the polygon
		}
	}

	return false;
}
