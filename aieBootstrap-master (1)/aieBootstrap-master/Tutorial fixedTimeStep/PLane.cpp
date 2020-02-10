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

void Plane::resolveCollision(Rigidbody* actor2)
{
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;
	float elasticity = 1;
	float j = glm::dot( -(1 + elasticity) * (actor2->getVelocity()), m_normal);

	glm::vec2 force = m_normal * j;
	applyForceToActor(actor2, force);
	
}