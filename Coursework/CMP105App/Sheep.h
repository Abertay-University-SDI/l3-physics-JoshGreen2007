#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"

class Sheep :
	public GameObject
{

public:
	Sheep();
	~Sheep();

	void handleInput(float dt) override;
	void update(float dt) override;

	void setWorldSize(float x, float y);
	void checkWallAndBounce();

	void collisionResponse(GameObject& collider) override;

private:

	void applyKnockback(const sf::Vector2f& normal);

	enum class Direction { UP, DOWN, LEFT, RIGHT, UP_RIGHT, DOWN_RIGHT, DOWN_LEFT, UP_LEFT, NONE };
	Direction m_direction = Direction::NONE;
	float m_speed = 300.0f;
	float m_inputBuffer = 0.f;
	const float INPUT_BUFFER_LENGTH = 0.1f;
	const float APPROX_ONE_OVER_ROOT_TWO = 0.70710678f;	// 1 / sqrt(2)
	const float CEOFF_OF_RESTITUTION = 0.8f;
	const float MINIMUM_VELOCITY = 200.0f;
	const float KNOCKBACK = 500.0f;
	sf::Vector2f m_worldSize;	// pass as param to constructor

	const float ACCELERATION = 500.f;   // rate of acceleration, suggested range 10-500
	const float DRAG_FACTOR = 0.95f;    // friction, suggest range 0.7-0.99
	sf::Vector2f m_acceleration;

	Animation m_walkDown;
	Animation m_walkUp;
	Animation m_walkUpRight;
	Animation m_walkRight;
	Animation m_walkDownRight;
	Animation* m_currentAnimation;
};