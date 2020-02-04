#include "PLane.h"
#include "Gizmos.h"
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