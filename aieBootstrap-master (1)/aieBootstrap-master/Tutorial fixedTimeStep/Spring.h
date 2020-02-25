#pragma once
#include "RigidBody.h"
class Spring : public Rigidbody
{
public:
	Spring(Rigidbody* body1, Rigidbody* body2, float restLength, float springCoefficient, float damping, glm::vec2 contact1 = glm::vec2(0, 0), glm::vec2 contact2 = glm::vec2(0, 0)) : Rigidbody(JOINT, glm::vec2(0, 0), glm::vec2(0, 0), 0.0f, 0.0f, 0.0f)
	{
		m_body1 = body1;
		m_body2 = body2;
		m_restLength = restLength;
		m_springCoefficient = springCoefficient;
		m_damping = damping;
		m_contact1 = contact1;
		m_contact2 = contact2;
	}
	virtual float getMomentFormula() { return 0; }
	bool virtual ContainsPoint(glm::vec2 point) const { return false; }
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void makeGizmo();

protected:
	Rigidbody* m_body1;
	Rigidbody* m_body2;

	glm::vec2 m_contact1;
	glm::vec2 m_contact2;

	float m_damping;
	float m_restLength;
	float m_springCoefficient;

};

