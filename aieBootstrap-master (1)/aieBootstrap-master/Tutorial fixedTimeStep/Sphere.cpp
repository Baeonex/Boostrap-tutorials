#include "Sphere.h"
#include "Gizmos.h"
Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : Rigidbody(SPHERE, position, velocity, 0, mass) { m_radius = radius;  m_colour = colour; }

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 20, m_colour);
}
bool Sphere::checkCollision(PhysicsObject* pOther)
{
	Sphere* sphere = dynamic_cast<Sphere*>(pOther);
	if (sphere != NULL)
	{

		if (glm::distance(m_position, sphere->m_position) < (m_radius + sphere->m_radius))
		{
			return true;
		}
		else return false;
	}
	else
		return false;
}

