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

void Rigidbody::resolveCollision(Rigidbody* actor2)
{
    glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
    glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;
    float elasticity = 1;
    float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
        glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->getMass())));
    glm::vec2 force = normal * j;
    applyForceToActor(actor2, -force);
}