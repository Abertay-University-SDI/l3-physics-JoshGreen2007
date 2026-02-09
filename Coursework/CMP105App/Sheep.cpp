#include <iostream>
#include <cmath>

#include "Pig.h"
#include "Sheep.h"
#include "Framework/Collision.h"

Sheep::Sheep()
{

	setCollisionBox({ {2.f , 2.f} , {60.f , 60.f} });
	setCollider(true);

	// initialise animations
	for (int i = 0; i < 4; i++)
		m_walkDown.addFrame({ { 64 * i, 0 }, { 64, 64 } });
	m_walkDown.setLooping(true);
	m_walkDown.setFrameSpeed(0.25f);

	for (int i = 0; i < 4; i++)
		m_walkUp.addFrame({ { (64 * (i + 4)), 0 }, { 64, 64 } });
	m_walkUp.setLooping(true);
	m_walkUp.setFrameSpeed(0.25f);

	for (int i = 0; i < 4; i++)
		m_walkUpRight.addFrame({ { 64 * i, 64 }, { 64, 64 } });
	m_walkUpRight.setLooping(true);
	m_walkUpRight.setFrameSpeed(0.25f);

	for (int i = 0; i < 4; i++)
		m_walkRight.addFrame({ { (64 * (i + 4)), 64 }, { 64, 64 } });
	m_walkRight.setLooping(true);
	m_walkRight.setFrameSpeed(0.25f);

	for (int i = 0; i < 4; i++)
		m_walkDownRight.addFrame({ { 64 * i, 128 }, { 64, 64 } });
	m_walkDownRight.setLooping(true);
	m_walkDownRight.setFrameSpeed(0.25f);

	// the next 4 animations go clockwise from Up through Right to Down.

	m_currentAnimation = &m_walkDown;
	setTextureRect(m_currentAnimation->getCurrentFrame());

}

Sheep::~Sheep()
{
}

void Sheep::handleInput(float dt)
{

	sf::Vector2f inputDir(0.f, 0.f);

	if (m_input->isKeyDown(sf::Keyboard::Scancode::W))
	{

		inputDir.y -= 1.f;

	}

	if (m_input->isKeyDown(sf::Keyboard::Scancode::A))
	{

		inputDir.x -= 1.f;

	}

	if (m_input->isKeyDown(sf::Keyboard::Scancode::S))
	{

		inputDir.y += 1.f;

	}

	if (m_input->isKeyDown(sf::Keyboard::Scancode::D))
	{

		inputDir.x += 1.f;

	}

	// Convert input into normalized (removes 1.4x speed increase on diagonal movement)
	if (inputDir.length() > 0) { inputDir = inputDir.normalized(); }

	// Convert direction into acceleration
	m_acceleration = inputDir * ACCELERATION;

}


void Sheep::update(float dt)
{

	// Apply acceleration to velocity
	m_velocity += m_acceleration * dt;

	// Apply Friction
	m_velocity *= DRAG_FACTOR;
	checkWallAndBounce();
	move(m_velocity * dt);

	sf::Vector2f pos = getPosition();
	sf::Vector2f size = getSize();

	// Apply knockback for when Sheep goes beyond world boundaries

	if (pos.x < 0)
	{

		applyKnockback({ 1.f, 0.f });

	}

	else if (pos.x + size.x > m_worldSize.x)
	{

		applyKnockback({ -1.f, 0.f });

	}

	if (pos.y < 0)
	{

		applyKnockback({ 0.f, 1.f });

	}

	else if (pos.y + size.y > m_worldSize.y)
	{

		applyKnockback({ 0.f, -1.f });

	}

	if (std::abs(m_velocity.x) > std::abs(m_velocity.y))
	{

		// After Lab: work on diagonal movement

		if (m_velocity.x > 0)
		{

			m_currentAnimation = &m_walkRight;
			m_currentAnimation->setFlipped(false);

		}

		else if (m_velocity.x < 0)
		{

			m_currentAnimation = &m_walkRight;
			m_currentAnimation->setFlipped(true);

		}

		if (m_velocity.y > 0)
		{

			m_currentAnimation = &m_walkDown;

		}

		else if (m_velocity.y < 0)
		{

			m_currentAnimation = &m_walkUp;

		}

		m_currentAnimation->animate(dt);
		setTextureRect(m_currentAnimation->getCurrentFrame());

	}

}

void Sheep::setWorldSize(float x, float y)
{

	m_worldSize = { x, y };

}

void Sheep::checkWallAndBounce()
{

	sf::Vector2f pos = getPosition();

	// If there is a wall collision instance, restitute velocity

	if ((pos.x < 0 && m_velocity.x < 0) ||
		(pos.x + getSize().x > m_worldSize.x &&
			m_velocity.x > 0))
	{

		std::cout << "Wall collision instance (X-axis)\n" << std::endl;
		m_velocity.x *= -CEOFF_OF_RESTITUTION;

	}

	if ((pos.y < 0 && m_velocity.y < 0) ||
		(pos.y + getSize().y > m_worldSize.y &&
			m_velocity.y > 0))
	{

		std::cout << "Wall collision instance (Y-axis)\n" << std::endl;
		m_velocity.y *= -CEOFF_OF_RESTITUTION;

	}

}

void Sheep::applyKnockback(const sf::Vector2f& normal)
{

	// Reflect velocity along the collision normal (dot product)
	float dot = (m_velocity.x * normal.x) + (m_velocity.y * normal.y);
	m_velocity -= 2.f * dot * normal;

	m_velocity *= CEOFF_OF_RESTITUTION;

	// Force a minimum speed at low velocity
	if (m_velocity.length() < MINIMUM_VELOCITY)
	{

		m_velocity = normal * KNOCKBACK;

	}

}

void Sheep::collisionResponse(GameObject& collider)
{

	std::cout << "Sheep collision instance\n" << std::endl;

	sf::Vector2f normal = getPosition() - collider.getPosition();

	if (normal.length() != 0.f)
	{

		normal = normal.normalized();

	}

	else
	{

		normal = { 0.f, 1.f };

	}

	applyKnockback(normal);

	if (m_velocity.length() < MINIMUM_VELOCITY)
	{

		m_velocity = m_velocity.normalized() * KNOCKBACK;

	}

}