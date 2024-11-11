#pragma once
#include "Rectangle.h"
#include <cmath>
#include <memory>

struct InputData;

class Game;

enum eDirection
{
    LEFT,
    RIGHT
};

class Player : public Rectangle
{
public:
    Player(Game* pGame);
    virtual ~Player() {}
    
    bool initialise();
    void move(InputData inputData, float deltaTime);
    // void attack();
    void update(float deltaTime);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void drawBoundingBox(sf::RenderWindow& window) const;
	sf::FloatRect getRotatedBounds() const;
	sf::Vector2f rotatePoint(const sf::Vector2f& point, float angle) const;

    bool isDead() const { return m_isDead; }
    void setIsDead(bool isDead) { m_isDead = isDead; }

	void addTrailSegment();
	bool isPathClosed();
	bool isPointInsideRectangle(const sf::Vector2f& point, const std::vector<sf::Vector2f>& rectangleCorners) const;

	std::vector<Rectangle> &getTrail() { return m_trail; }
	bool getIsClosed() { return m_isClosed; }
	bool checkPath();
	void eraseTrail();
	void lateUpdate(float deltaTime);

private:
    bool    m_isDead = false;
    eDirection m_direction = LEFT;
    Game*   m_pGame;

	float m_angle;
	std::vector<Rectangle> m_trail;
	bool m_isClosed;
	bool m_hasLeftFirstSquare;

	float m_trailTimer = 0.0f;  // Timer for trail segment creation
    float m_trailInterval = 0.1f;  // Interval for adding new trail segments (in seconds)
};
