#include "RigidBody.h"



 void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
     applyForce(gravity * m_mass * timeStep);  
     m_position += m_velocity * timeStep;
 }
 void Rigidbody::debug()
{

 }
void Rigidbody::applyForce(glm::vec2 force){
    glm::vec2 acceleration = force / m_mass;
    m_velocity += acceleration;
}
void Rigidbody::applyForceToActor(Rigidbody* actor2, glm::vec2 force){
    actor2->applyForce(force);
    applyForce(-force);
}

