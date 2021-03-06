#include "Box.h"
#include "Gizmos.h"
#include <iostream>
void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::fixedUpdate(gravity, timeStep);

	//store the local axes
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void Box::makeGizmo()
{
	// if only using rotation
// glm::mat4 transform = glm::rotate(m_rotation, glm::vec3(0, 0, 1));
// aie::Gizmos::add2DAABBFilled(getCenter(),
// m_extents, m_colour, &transform);
// draw using local axes
	glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;
	aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);

}

// check if any of the other box's corners are inside this box
bool Box::checkBoxCorners(Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal)
{
	float minX, maxX, minY, maxY;
	float boxW = box.getExtents().x * 2;
	float boxH = box.getExtents().y * 2;
	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);
	bool first = true;
	for (float x = -box.getExtents().x; x < boxW; x += boxW)
	{
		for (float y = -box.getExtents().y; y < boxH; y += boxH)
		{
			// position in worldspace
			glm::vec2 p = box.getPosition() + x * box.m_localX + y * box.m_localY;
			// position in our box's space
			glm::vec2 p0(glm::dot(p - m_position, m_localX),
				glm::dot(p - m_position, m_localY));
			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;
			if (p0.x >= -m_extents.x && p0.x <= m_extents.x &&
				p0.y >= -m_extents.y && p0.y <= m_extents.y)
			{
				numLocalContacts++;
				localContact += p0;
			}
			first = false;
		}
	}
	if (maxX <-m_extents.x || minX >m_extents.x ||
		maxY<-m_extents.y || minY >m_extents.y)
		return false;
	if (numLocalContacts == 0)
		return false;
	bool res = false;
	contact += m_position + (localContact.x * m_localX + localContact.y * m_localY) /
		(float)numLocalContacts;
	numContacts++;
	float pen0 = m_extents.x - minX;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = m_localX;
		pen = pen0;
		res = true;
	}
	pen0 = maxX + m_extents.x;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = -m_localX;
		pen = pen0;
		res = true;
	}
	pen0 = m_extents.y - minY;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = m_localY;
		pen = pen0;
		res = true;
	}
	pen0 = maxY + m_extents.y;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = -m_localY;
		pen = pen0;
		res = true;
	}
	return res;
}

bool Box::ContainsPoint(glm::vec2 point) const
{
	// Convert the point to box local space
	glm::vec2 newPoint(glm::dot(point - m_position, m_localX), glm::dot(point - m_position, m_localY));

	return newPoint.x < m_extents.x && newPoint.x > -m_extents.x &&
		newPoint.y < m_extents.y && newPoint.y > -m_extents.y;
}