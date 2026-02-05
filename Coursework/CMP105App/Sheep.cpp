#include "Sheep.h"

Sheep::Sheep()
{
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

	// Convert direction into acceleration
	m_acceleration = inputDir * ACCELERATION;

}


void Sheep::update(float dt)
{

	// Apply acceleration to velocity
	m_velocity += m_acceleration * dt;

	// Apply Friction
	m_velocity *= DRAG_FACTOR;

	move(m_velocity * dt);

	sf::Vector2f pos = getPosition();
	sf::Vector2f size = getSize();

	if (pos.x < 0 || pos.x + size.x > m_worldSize.x)
	{

		m_velocity.x = -m_velocity.x * CEOFF_OF_RESTITUTION;

	}

	if (pos.y < 0 || pos.y + size.y > m_worldSize.y)
	{

		m_velocity.y = -m_velocity.y * CEOFF_OF_RESTITUTION;

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