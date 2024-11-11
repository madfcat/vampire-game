#include "Player.h"
#include "InputHandler.h"
#include "Constants.h"
#include <vector>
#include "Game.h"

Player::Player(Game* pGame) :
    Rectangle(sf::Vector2f(PlayerWidth, PlayerHeight)),
    m_pGame(pGame),
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

	m_isClosed = false;
	m_hasLeftFirstSquare = false;

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

	// Update the trail
	m_trailTimer += deltaTime;
	if (m_trailTimer >= m_trailInterval) {
		addTrailSegment();
		m_trailTimer = 0.0f;  // Reset timer after adding a segment
	}
}

bool Player::checkPath()
{
	if (isPathClosed())
	{
		m_isClosed = true;
		std::cout << "Path closed" << std::endl;
	}
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

void Player::lateUpdate(float deltaTime)
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
    for (const auto& segment : m_trail) {
        target.draw(segment, states);
    }
	// Draw the player sprite
    Rectangle::draw(target, states);

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

void Player::addTrailSegment() {
	// std::cout << "addTrailSegment()" << std::endl;
    Rectangle segment(sf::Vector2f(getSize().x, getSize().y), getPosition());
    // segment.setPosition(getPosition());
	segment.setOrigin(getOrigin());
    segment.setRotation(getRotation());  // Set the rotation to match the player’s rotation
    segment.getRectangleShape().setFillColor(sf::Color(100, 100, 255, 150));  // Semi-transparent color for trail
	segment.getRectangleShape().setRotation(getRotation());
	if (m_trail.size() == 0 || segment.getPosition() != m_trail.back().getPosition())
    	m_trail.push_back(segment);
}

bool Player::isPathClosed() {
    // Ensure we have at least 2 segments to start
    if (m_trail.size() < 2) {
        return false;  // Not enough segments for a loop
    }

    // Get the position of the first trail square (using m_trail[0])
    const sf::Vector2f& firstPosition = m_trail.front().getPosition();
    const sf::Vector2f& lastPosition = m_trail.back().getPosition();

    // Get the global bounds of the first square
    sf::FloatRect firstSquareBounds = const_cast<Rectangle&>(m_trail.front()).getRectangleShape().getGlobalBounds();

    // Check if the player's current position (origin) is inside the first square's bounds
    if (!firstSquareBounds.contains(getPosition() + sf::Vector2f(0, getSize().y / 2))) {
        m_hasLeftFirstSquare = true;  // Player's origin is outside the first square, so it has left
    }
	// std::cout << "m_hasLeftFirstSquare: " << m_hasLeftFirstSquare << std::endl;

	bool enteredFirstSquareAgain = false;
	if (m_hasLeftFirstSquare && firstSquareBounds.contains(getPosition() + sf::Vector2f(0, getSize().y / 2)))
	{
		enteredFirstSquareAgain = true;
	}
	// std::cout << "enteredFirstSquareAgain: " << enteredFirstSquareAgain << std::endl;

    // If we've left the first square, check if we've returned to it
    if (enteredFirstSquareAgain) {
		m_hasLeftFirstSquare = false;
		return true;
        // // Compare the player's position (lastPosition) to the first square's position
        // float distance = sqrt((lastPosition.x - firstPosition.x) * (lastPosition.x - firstPosition.x) +
        //                       (lastPosition.y - firstPosition.y) * (lastPosition.y - firstPosition.y));
        
        // float distanceThreshold = 10.0f;  // Adjust this threshold as necessary
        // return distance < distanceThreshold;  // Path is closed if we're back near the first square
    }

    return false;  // Path is not closed yet
}

// bool Player::isPathClosed() const {
//     if (m_trail.size() < 200) {
//         return false;  // Not enough points to form a closed path
//     }

//     // Check if the last rectangle collides with the first rectangle
//     return m_trail.back().collidesWith(&m_trail.front());
// }