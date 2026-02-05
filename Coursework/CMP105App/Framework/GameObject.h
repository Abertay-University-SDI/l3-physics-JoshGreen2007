// Game Object Class
// A class representing a single renderable game object.
// Game Object extend sf::RectangleShape, and added functionailty for collision, sprite states (alive/dead), velocity for movement, input component
// Most elements of your game will extend game object to create larger more complex elements such as player, enemies, bullets etc.
// @author paul Robertson
// @reviser William Kavanagh (2025)

#pragma once
// Disable the warning for the SFML headers
#pragma warning(push)
#pragma warning(disable : 4275 4251)
#include <SFML/Graphics.hpp>
#pragma warning(pop)
#include "Input.h"

class GameObject : public sf::RectangleShape
{
public:
	GameObject();
	virtual ~GameObject() = default;

	// Virtual update function. Base function does nothing (for static objects). Inheritted version will most likely override this function.
	virtual void handleInput(float dt);
	virtual void update(float dt);

	// Control sprite speed and direction
	void setVelocity(sf::Vector2f vel);
	void setVelocity(float vx, float vy);
	sf::Vector2f getVelocity() const;

	// Sprite state
	bool isAlive() const { return m_alive; };
	void setAlive(bool b) { m_alive = b; };

	// For sprite collision, set collider box, get collider box, and dedicated virtual function for collision response
	bool isCollider() const { return m_collider; };
	void setCollider(bool b) { m_collider = b; };
	sf::FloatRect getCollisionBox() const;
	void setCollisionBox(float x, float y, float width, float height);
	void setCollisionBox(sf::FloatRect fr);
	virtual void collisionResponse(GameObject& collider);

	// Set the input component
	void setInput(Input* in) { m_input = in; };
	void setWindow(sf::RenderWindow* win) { m_window = win; };

protected:
	// Sprite properties
	sf::Vector2f m_velocity;
	bool m_alive;

	// Collision 
	sf::FloatRect m_collisionBox;
	bool m_collider;

	// input component
	// NOTE: we are using pointers here because GameObject may not need them, or they may be assigned later.
	Input* m_input;
	sf::RenderWindow* m_window;

};