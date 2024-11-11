#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Constants.h"
#include <iostream>
#include <vector>

class Rectangle : public sf::Drawable, public sf::Transformable
{
public:
    Rectangle(sf::Vector2f size, sf::Vector2f position = sf::Vector2f(0, 0));
    virtual ~Rectangle() {}
    
    bool collidesWith(const Rectangle* pOther) const;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Vector2f getCenter();

    void setSize(sf::Vector2f size) { m_size = size; }
    sf::Vector2f getSize() const { return m_size; }

	sf::FloatRect getRotatedBounds() const;
	sf::Vector2f rotatePoint(const sf::Vector2f& point, float angle) const;

    void setColor(sf::Color color) { 
		m_color = color;
		m_rectangleShape.setFillColor(color);
	 }
    sf::Color getColor() const { return m_color; }

	std::vector<sf::Vector2f> getCorners() const;
	std::vector<sf::Vector2f> getAxes(const std::vector<sf::Vector2f>& thisCorners, const std::vector<sf::Vector2f>& otherCorners) const;
	bool projectionsOverlap(const std::vector<sf::Vector2f>& thisCorners, const std::vector<sf::Vector2f>& otherCorners, const sf::Vector2f& axis) const;
	sf::RectangleShape &getRectangleShape() { return m_rectangleShape; }

protected:
    sf::Sprite m_sprite;
	sf::RectangleShape m_rectangleShape;

private:
    sf::Color m_color = sf::Color::White;
    sf::Vector2f m_size;
};
