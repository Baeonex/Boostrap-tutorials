#pragma once
#include "RigidBody.h"
class Box : public Rigidbody
{
public:
	Box(float width, float height, float mass, glm::vec2 position, glm::vec4 colour, glm::vec2 velocity) : Rigidbody(BOX, position, velocity, 0, mass)
	{
		m_width = width;
		m_height = height;
		m_colour = colour;
		m_extents = glm::vec2(m_width / 2, m_height / 2);
		m_localX = glm::vec2(0, 0);
		m_localY = glm::vec2(0, 0);
	}

	bool checkBoxCorners(Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);
	virtual void makeGizmo();
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);

	float getHeight() { return m_height; }
	float getWidth() { return m_width; }
	glm::vec2 getExtents() { return m_extents; }
	void setColour(glm::vec4 colour) { m_colour = colour; }
	glm::vec2 getLocalX() { return m_localX; }
	glm::vec2 getLocalY() { return m_localY; }


private:
	float m_width;
	float m_height;

	
	glm::vec2 m_extents; //halfway lengths
	glm::vec4 m_colour;

	//store the local x,y axis of the box based on its angle of rotation
	glm::vec2 m_localX;
	glm::vec2 m_localY;
};

