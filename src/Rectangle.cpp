
#include "Rectangle.h"
#include <SFML/Graphics.hpp>

Rectangle::Rectangle(sf::Vector2f size, sf::Vector2f position) : m_size(size)
{
	    // Set the position and size of the rectangle
    setSize(size);
    setPosition(position);

	setOrigin(size.x / 2, size.y / 2);
    m_rectangleShape.setSize(getSize());
    m_rectangleShape.setPosition(getPosition());
	m_rectangleShape.setOrigin(getOrigin());

    // Set the color of the rectangle
    m_rectangleShape.setFillColor(m_color);  // Set the color (default is white)

    // sf::Transformable::setPosition(position);
}

// // Collision detection
// bool Rectangle::collidesWith(Rectangle* pOther)
// {
//     // Get the rotated bounds of the current and other rectangles
//     sf::FloatRect thisBounds = getRotatedBounds();
//     sf::FloatRect otherBounds = pOther->getRotatedBounds();

//     // Check if the bounds overlap
//     if (thisBounds.intersects(otherBounds)) {
//         return true; // Rectangles overlap
//     }
//     return false; // No collision
// }

// sf::FloatRect Rectangle::getRotatedBounds() const
// {
//     // Get the current size and position of the rectangle
//     sf::Vector2f size = getSize();
//     sf::Vector2f position = getPosition();

//     // Get the rotation angle
//     float angle = getRotation();

//     // Calculate the half-width and half-height
//     float halfWidth = size.x / 2.f;
//     float halfHeight = size.y / 2.f;

//     // Calculate the rotated corners of the rectangle
//     // The rectangle has four corners, and we need to rotate each corner
//     sf::Vector2f topLeft(-halfWidth, -halfHeight);
//     sf::Vector2f topRight(halfWidth, -halfHeight);
//     sf::Vector2f bottomLeft(-halfWidth, halfHeight);
//     sf::Vector2f bottomRight(halfWidth, halfHeight);

//     // Apply rotation to each corner
//     topLeft = rotatePoint(topLeft, angle);
//     topRight = rotatePoint(topRight, angle);
//     bottomLeft = rotatePoint(bottomLeft, angle);
//     bottomRight = rotatePoint(bottomRight, angle);

//     // Get the bounds that enclose all the rotated points
//     float minX = std::min({topLeft.x, topRight.x, bottomLeft.x, bottomRight.x});
//     float maxX = std::max({topLeft.x, topRight.x, bottomLeft.x, bottomRight.x});
//     float minY = std::min({topLeft.y, topRight.y, bottomLeft.y, bottomRight.y});
//     float maxY = std::max({topLeft.y, topRight.y, bottomLeft.y, bottomRight.y});

//     // Return the rotated bounding box
//     return sf::FloatRect(position.x + minX, position.y + minY, maxX - minX, maxY - minY);
// }

// sf::Vector2f Rectangle::rotatePoint(const sf::Vector2f& point, float angle) const
// {
//     // Convert angle to radians
//     float radians = angle * (M_PI / 180.f);
    
//     // Apply rotation matrix to rotate the point
//     float cosAngle = std::cos(radians);
//     float sinAngle = std::sin(radians);

//     // Rotate point
//     return sf::Vector2f(
//         point.x * cosAngle - point.y * sinAngle,
//         point.x * sinAngle + point.y * cosAngle
//     );
// }


// // Collision detection working for squares
// bool Rectangle::collidesWith(Rectangle* pOther)
// {
//     sf::Vector2f thisPosition = getPosition();
//     sf::Vector2f thisSize = getSize();
// 	sf::Vector2f thisOriginOffset = getOrigin();
// 	thisPosition = thisPosition - thisOriginOffset;

//     sf::Vector2f otherPosition = pOther->getPosition();
//     sf::Vector2f otherSize = pOther->getSize();

// 	sf::Vector2f otherOriginOffset = getOrigin();
// 	// std::cout << "otherPosition.x before: " << otherPosition.x << std::endl;
// 	// std::cout << "otherPosition.y before: " << otherPosition.y << std::endl;
// 	// std::cout << "otherOriginOffset.x: " << otherOriginOffset.x << std::endl;
// 	// std::cout << "otherOriginOffset.y: " << otherOriginOffset.y << std::endl;
// 	otherPosition = otherPosition - otherOriginOffset;  // Adjust for origin offset
// 	// std::cout << "otherPosition.x after: " << otherPosition.x << std::endl;
// 	// std::cout << "otherPosition.y after: " << otherPosition.y << std::endl;

//     float thisLeft = thisPosition.x;
//     float thisRight = thisPosition.x + thisSize.x;
//     float thisTop = thisPosition.y;
//     float thisBottom = thisPosition.y + thisSize.y;

//     float otherLeft = otherPosition.x;
//     float otherRight = otherPosition.x + otherSize.x;
//     float otherTop = otherPosition.y;
//     float otherBottom = otherPosition.y + otherSize.y;

//     if (thisLeft < otherRight && thisRight > otherLeft &&
//         thisTop < otherBottom && thisBottom > otherTop) {
//         // Rectangles overlap
//         return true;
//     }
//     return false;
// }

bool Rectangle::collidesWith(const Rectangle* pOther) const
{
    sf::Vector2f thisPosition = getPosition();
    sf::Vector2f thisSize = getSize();
    sf::Vector2f thisOriginOffset = getOrigin();
    thisPosition = thisPosition - thisOriginOffset;  // Adjust for origin offset

    sf::Vector2f otherPosition = pOther->getPosition();
    sf::Vector2f otherSize = pOther->getSize();
    sf::Vector2f otherOriginOffset = pOther->getOrigin();
    otherPosition = otherPosition - otherOriginOffset;  // Adjust for origin offset

	std::vector<sf::Vector2f> thisCorners = getCorners();
	std::vector<sf::Vector2f> otherCorners = pOther->getCorners();

    // Get the axes to check: 4 axes, one for each edge of the rectangles
    std::vector<sf::Vector2f> axes = getAxes(thisCorners, otherCorners);

    // For each axis, check if the projections of the two rectangles overlap
    for (const auto& axis : axes) {
        if (!projectionsOverlap(thisCorners, otherCorners, axis)) {
            return false;  // No collision if projections do not overlap on any axis
        }
    }

    return true;  // If projections overlap on all axes, there's a collision
}

std::vector<sf::Vector2f> Rectangle::getCorners() const
{
    std::vector<sf::Vector2f> corners(4);

    // Get rectangle properties
    sf::Vector2f size = getSize();
    sf::Vector2f position = getPosition();  // Position of the rectangle
    float rotation = getRotation();         // Rotation angle in degrees

    // Calculate the center of the rectangle (position should be its center point)
    sf::Vector2f center = position + getOrigin();

    // Define the unrotated corners relative to the center
    std::vector<sf::Vector2f> localCorners = {
        {-size.x / 2, -size.y / 2}, // Top-left
        { size.x / 2, -size.y / 2}, // Top-right
        { size.x / 2,  size.y / 2}, // Bottom-right
        {-size.x / 2,  size.y / 2}  // Bottom-left
    };

    // Convert the rotation to radians
    float angleRad = rotation * (M_PI / 180.0f);

    // Apply rotation and add the center position to each corner
    for (int i = 0; i < 4; ++i) {
        float rotatedX = localCorners[i].x * cos(angleRad) - localCorners[i].y * sin(angleRad);
        float rotatedY = localCorners[i].x * sin(angleRad) + localCorners[i].y * cos(angleRad);

        corners[i] = sf::Vector2f(rotatedX, rotatedY) + center;
    }

    return corners;
}

std::vector<sf::Vector2f> Rectangle::getAxes(const std::vector<sf::Vector2f>& thisCorners, const std::vector<sf::Vector2f>& otherCorners) const
{
    std::vector<sf::Vector2f> axes;

    // For each pair of consecutive corners of both rectangles, calculate the edge's perpendicular vector (axis)
    for (size_t i = 0; i < 4; ++i) {
        // Get the edge of the first rectangle
        sf::Vector2f edge = thisCorners[(i + 1) % 4] - thisCorners[i];
        // Perpendicular to the edge is the axis
        axes.push_back(sf::Vector2f(-edge.y, edge.x));

        // Get the edge of the second rectangle
        edge = otherCorners[(i + 1) % 4] - otherCorners[i];
        axes.push_back(sf::Vector2f(-edge.y, edge.x));
    }

    return axes;
}

bool Rectangle::projectionsOverlap(const std::vector<sf::Vector2f>& thisCorners, const std::vector<sf::Vector2f>& otherCorners, const sf::Vector2f& axis) const
{
    // Project the corners of both rectangles onto the axis and check for overlap
    float thisMin = std::numeric_limits<float>::max();
    float thisMax = std::numeric_limits<float>::min();

    float otherMin = std::numeric_limits<float>::max();
    float otherMax = std::numeric_limits<float>::min();

    // Project the first rectangle onto the axis
    for (const auto& corner : thisCorners) {
        float projection = corner.x * axis.x + corner.y * axis.y;
        thisMin = std::min(thisMin, projection);
        thisMax = std::max(thisMax, projection);
    }

    // Project the second rectangle onto the axis
    for (const auto& corner : otherCorners) {
        float projection = corner.x * axis.x + corner.y * axis.y;
        otherMin = std::min(otherMin, projection);
        otherMax = std::max(otherMax, projection);
    }

    // If projections do not overlap, return false
    return !(thisMax < otherMin || otherMax < thisMin);
}

void Rectangle::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	// Draw the rectangle shape
	target.draw(m_rectangleShape, states);

    target.draw(m_sprite);
}

sf::Vector2f Rectangle::getCenter()
{
    return getPosition() + (m_size * 0.5f);
}
