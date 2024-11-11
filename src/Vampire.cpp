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
	// setOrigin(sf::Vector2f(0.0f, 0.0f));
	setOrigin(sf::Vector2f(VampireWidth / 2, VampireHeight / 2));
	setIsKilled(false);

	m_sprite.setTexture(*m_pGame->getVampireTexture());
	sf::Vector2u textureSize = (*m_pGame->getVampireTexture()).getSize();
	m_sprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
	m_sprite.setScale(VampireWidth / textureSize.x, VampireWidth / textureSize.y);
}

void Vampire::update(float deltaTime)
{
	if (m_isKilled)
		return;

	Player *pPlayer = m_pGame->getPlayer();

	if (isEnemyBetweenTrailPoints(pPlayer))
	{
		m_pGame->addEnemiesKilled(1);
		std::cout << "Killed: "<< m_pGame->getEnemiesKilled() << std::endl;
		setIsKilled(true);
		return;
	}

	// Collides with trail
	// if (collidesWith(pPlayer->getWeapon()))
	// {
	//     setIsKilled(true);
	//     return;
	// }

	if (collidesWith(pPlayer))
		pPlayer->setIsDead(true);

	sf::Vector2f playerCenter = pPlayer->getCenter();
	sf::Vector2f direction = VecNormalized(playerCenter - getCenter());
	direction *= VampireSpeed * deltaTime;
	sf::Transformable::move(direction);
	m_rectangleShape.setPosition(getPosition().x, getPosition().y);
	m_rectangleShape.setSize(getSize());
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

// bool Vampire::isEnemyBetweenTrailPoints(const Player* pPlayer) const
// {
// 	const auto & trail = pPlayer->getTrail();
// 	if (trail.size() < 2)
// 		return false;
//     for (size_t i = 0; i < trail.size() - 1; ++i) {
//         const sf::Vector2f& point1 = trail[i].getPosition();
//         const sf::Vector2f& point2 = trail[i + 1].getPosition();

//         // Check if the enemy's x and y coordinates are between the x/y of the two points
//         if ((getPosition().x > std::min(point1.x, point2.x) && getPosition().x < std::max(point1.x, point2.x)) &&
//             (getPosition().y > std::min(point1.y, point2.y) && getPosition().y < std::max(point1.y, point2.y))) {
//             return true;
//         }
//     }

//     return false;
// }