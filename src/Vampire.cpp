#include "Vampire.h"
#include "Constants.h"
#include "Game.h"
#include "Weapon.h"
#include "Player.h"
#include "MathUtils.h"

Vampire::Vampire(Game* game, sf::Vector2f position) :
    Rectangle(sf::Vector2f(VampireWidth, VampireHeight)),
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
    
    Player* pPlayer = m_pGame->getPlayer();

	if (isEnemyBetweenTrailPoints(pPlayer))
	{
		setIsKilled(true);
		return;
	}

    if (collidesWith(pPlayer->getWeapon()))
    {
        setIsKilled(true);
        return;
    }

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

bool Vampire::isEnemyBetweenTrailPoints(Player* pPlayer) {
	
    auto& trail = pPlayer->getTrail();
    if (trail.size() < 2)
        return false;

	if (pPlayer->getIsClosed())
	{
		const sf::Vector2f& enemyPosition = getPosition();  // Get the position of the enemy (Vampire)

		for (size_t i = 0; i < trail.size(); ++i) {
			const sf::Vector2f& currentPoint = trail[i].getPosition();

			// Check all points before the current one
			for (size_t j = 0; j < i; ++j) {
				const sf::Vector2f& previousPoint = trail[j].getPosition();

				// Check if the enemy is between 'previousPoint' and 'currentPoint'
				if ((enemyPosition.x > std::min(previousPoint.x, currentPoint.x) && enemyPosition.x < std::max(previousPoint.x, currentPoint.x)) &&
					(enemyPosition.y > std::min(previousPoint.y, currentPoint.y) && enemyPosition.y < std::max(previousPoint.y, currentPoint.y))) {
					return true;
				}
			}

			// Check all points after the current one
			for (size_t j = i + 1; j < trail.size(); ++j) {
				const sf::Vector2f& nextPoint = trail[j].getPosition();

				// Check if the enemy is between 'currentPoint' and 'nextPoint'
				if ((enemyPosition.x > std::min(currentPoint.x, nextPoint.x) && enemyPosition.x < std::max(currentPoint.x, nextPoint.x)) &&
					(enemyPosition.y > std::min(currentPoint.y, nextPoint.y) && enemyPosition.y < std::max(currentPoint.y, nextPoint.y))) {
					return true;
				}
			}
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