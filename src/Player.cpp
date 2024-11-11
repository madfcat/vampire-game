#include "Player.h"
#include "Weapon.h"
#include "InputHandler.h"
#include "Constants.h"
#include <vector>
#include "Game.h"

Player::Player(Game* pGame) :
    Rectangle(sf::Vector2f(PlayerWidth, PlayerHeight)),
    m_pGame(pGame),
    m_pWeapon(std::make_unique<Weapon>()),
	m_angle(-M_PI / 2)
{
    // setOrigin(sf::Vector2f(0.0f, 0.0f));
    setOrigin(sf::Vector2f(PlayerWidth / 2, PlayerHeight / 2));
	
}

bool Player::initialise()
{
    m_sprite.setTexture(*m_pGame->getPlayerTexture());
	sf::Vector2u textureSize = m_sprite.getTexture()->getSize();
	m_sprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
	m_sprite.setScale(getSize().x / textureSize.x, getSize().y / textureSize.y);
    // m_sprite.setScale(3.5f, 3.5f);
	setColor(sf::Color::Green);
    setIsDead(false);
    setPosition(ScreenWidth / 2, ScreenHeight / 2);
    m_sprite.setPosition(getPosition());
	// m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);

    return true;
}

void Player::move(InputData inputData, float deltaTime)
{
    float xSpeed = 0.0f;
    float ySpeed = 0.0f;

	m_angle -= RotationSpeed * deltaTime * inputData.m_movingLeft;
	m_angle += RotationSpeed * deltaTime * inputData.m_movingRight;

    if (m_angle < 0) m_angle += 2 * M_PI;
    if (m_angle >= 2 * M_PI) m_angle -= 2 * M_PI;

	sf::Vector2f direction(std::cos(m_angle), std::sin(m_angle));

	if (inputData.m_movingUp) {
        float speed = PlayerSpeed * deltaTime;
		m_sprite.setRotation(m_angle * 180 / M_PI + 90);
		// m_sprite.setRotation(m_angle * 180 / M_PI);
        sf::Transformable::move(direction * speed);
    	setPosition(std::clamp(getPosition().x, 0.0f, (float)ScreenWidth), getPosition().y);
		setRotation(m_angle * 180 / M_PI);
		m_rectangleShape.setPosition(getPosition().x, getPosition().y);
		m_rectangleShape.setSize(getSize());
		m_rectangleShape.setRotation(getRotation());
    }

    // xSpeed -= inputData.m_movingLeft * PlayerSpeed;
    // xSpeed += inputData.m_movingRight * PlayerSpeed;
    // xSpeed *= deltaTime;

    // ySpeed -= inputData.m_movingUp * PlayerSpeed;
    // ySpeed += inputData.m_movingDown * PlayerSpeed;
    // ySpeed *= deltaTime;
    
    // sf::Transformable::move(sf::Vector2f(xSpeed, ySpeed));
    // setPosition(std::clamp(getPosition().x, 0.0f, (float)ScreenWidth), getPosition().y);

    if (m_pWeapon->isActive() == false)
    {
        if (inputData.m_movingLeft == true && inputData.m_movingRight == false)
            m_direction = LEFT;
        else if (inputData.m_movingLeft == false && inputData.m_movingRight == true)
            m_direction = RIGHT;
    }
}

void Player::attack()
{
    m_pWeapon->setActive(true);
}

void Player::update(float deltaTime)
{
    sf::Vector2f weaponSize = m_pWeapon->getSize();

    m_sprite.setPosition(getPosition());
    m_pWeapon->setPosition(sf::Vector2f(
        getCenter().x - (m_direction == LEFT ? weaponSize.x : 0.0f),
        getCenter().y - weaponSize.y / 2.0f));
    m_pWeapon->update(deltaTime);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    Rectangle::draw(target, states);
    m_pWeapon->draw(target, states);

	drawBoundingBox(static_cast<sf::RenderWindow&>(target));
}

void Player::drawBoundingBox(sf::RenderWindow& window) const
{
    // Get the global bounds of the sprite (includes scaling, rotation, etc.)
    sf::FloatRect spriteBounds = getRotatedBounds();

    // Create a rectangle to represent the bounding box
    sf::RectangleShape boundingBox(sf::Vector2f(spriteBounds.width, spriteBounds.height));

    // Set the position of the bounding box (align it with the sprite)
    boundingBox.setPosition(spriteBounds.left, spriteBounds.top);

    // Set the color of the bounding box (red for visibility)
    boundingBox.setFillColor(sf::Color::Transparent);  // Transparent fill
    boundingBox.setOutlineColor(sf::Color::Red);      // Red outline for the bounding box
    boundingBox.setOutlineThickness(2);               // Set outline thickness for visibility

    // Draw the bounding box
    window.draw(boundingBox);
}

sf::FloatRect Player::getRotatedBounds() const
{
    // Get the center of the sprite
    sf::Vector2f center = m_sprite.getPosition();
    
    // Get the 4 corners of the sprite in its local coordinate system
    sf::Vector2f topLeft(-m_sprite.getLocalBounds().width / 2, -m_sprite.getLocalBounds().height / 2);
    sf::Vector2f topRight(m_sprite.getLocalBounds().width / 2, -m_sprite.getLocalBounds().height / 2);
    sf::Vector2f bottomLeft(-m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);
    sf::Vector2f bottomRight(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);

    // Get the rotation of the sprite
    float angle = m_sprite.getRotation();

    // Rotate each corner point based on the sprite's rotation
    topLeft = rotatePoint(topLeft, angle);
    topRight = rotatePoint(topRight, angle);
    bottomLeft = rotatePoint(bottomLeft, angle);
    bottomRight = rotatePoint(bottomRight, angle);

    // Translate the rotated points back to the global position of the sprite
    topLeft += center;
    topRight += center;
    bottomLeft += center;
    bottomRight += center;

    // Calculate the bounding box of the rotated sprite
    float minX = std::min({topLeft.x, topRight.x, bottomLeft.x, bottomRight.x});
    float maxX = std::max({topLeft.x, topRight.x, bottomLeft.x, bottomRight.x});
    float minY = std::min({topLeft.y, topRight.y, bottomLeft.y, bottomRight.y});
    float maxY = std::max({topLeft.y, topRight.y, bottomLeft.y, bottomRight.y});

    // Return the bounding box as a FloatRect
    return sf::FloatRect(minX, minY, maxX - minX, maxY - minY);
}

sf::Vector2f Player::rotatePoint(const sf::Vector2f& point, float angle) const
{
    // Convert angle to radians
    float radians = angle * M_PI / 180.f;
    
    // Rotate the point using the 2D rotation matrix
    float cosAngle = std::cos(radians);
    float sinAngle = std::sin(radians);

    // Apply rotation
    return sf::Vector2f(
        cosAngle * point.x - sinAngle * point.y,
        sinAngle * point.x + cosAngle * point.y
    );
}