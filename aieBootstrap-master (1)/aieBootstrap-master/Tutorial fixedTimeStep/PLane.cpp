#include "PLane.h"
#include "Gizmos.h"
#include <iostream>
using namespace std;
void Plane::makeGizmo()
{
	float lineSegmentLength = 300;  
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin; 
	// easy to rotate normal through 90 degrees around z  
	glm::vec2 parallel(m_normal.y, -m_normal.x); 
	glm::vec4 colour(1, 1, 1, 1); 
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength); 
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength); 
	aie::Gizmos::add2DLine(start, end, colour);
	
}

void Plane::resolveCollision(Rigidbody* actor2, glm::vec2 contact)
{
	glm::vec2 normal = m_normal;
	glm::vec2 relativeVelocity = actor2->getVelocity();
	float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / (1 / actor2->getMass());
		
	glm::vec2 force = normal * j;
	actor2->applyForce(force, contact - actor2->getPosition());


	
}
bool Plane::ContainsPoint(glm::vec2 point) const
{
	return false;
}