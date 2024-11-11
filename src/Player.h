#pragma once
#include "Rectangle.h"
#include <cmath>
#include <memory>

struct InputData;

class Game;
class Weapon;

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
    void attack();
    void update(float deltaTime);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void drawBoundingBox(sf::RenderWindow& window) const;
	sf::FloatRect getRotatedBounds() const;
	sf::Vector2f rotatePoint(const sf::Vector2f& point, float angle) const;

    bool isDead() const { return m_isDead; }
    void setIsDead(bool isDead) { m_isDead = isDead; }

    Weapon* getWeapon() { return m_pWeapon.get(); }

	void addTrailSegment();
	bool isPathClosed() const;

private:
    bool    m_isDead = false;
    eDirection m_direction = LEFT;
    Game*   m_pGame;
    std::unique_ptr<Weapon> m_pWeapon;

	float m_angle;
	std::vector<Rectangle> m_trail;
	// bool m_isClosed;
};
