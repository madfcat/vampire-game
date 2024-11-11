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