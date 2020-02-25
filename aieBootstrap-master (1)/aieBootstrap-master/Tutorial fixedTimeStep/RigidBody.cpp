#include "RigidBody.h"
const float MIN_LINEAR_THRESHOLD = 0;
const float MIN_ROTATION_THRESHOLD = 0;


void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
    if (m_isKinematic)
        return;
    // remember when applying the force of gravity, mass cancels out
    m_velocity += gravity * timeStep;
    m_position += m_velocity * timeStep;
    m_velocity -= m_velocity * m_linearDrag * timeStep;
    m_rotation +=  m_angularVelocity * timeStep;
    m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;
    if (length(m_velocity) < MIN_LINEAR_THRESHOLD) {
        if (length(m_velocity) < length(gravity) * m_linearDrag * timeStep)
            m_velocity = glm::vec2(0, 0);
    }
    if (abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD) {
        m_angularVelocity = 0;
    }
}
 void Rigidbody::debug()
{

 }
void Rigidbody::applyForce(glm::vec2 force, glm::vec2 pos){

    glm::vec2 acceleration = force / m_mass;
    m_velocity += acceleration;
    m_angularVelocity += (force.y * pos.x - force.x * pos.y) / (m_moment);

    /*
    glm::vec2 momentArm = pos - m_position;
    glm::vec2 parallelComp = momentArm * (glm::dot(force, momentArm) / glm::dot(momentArm, momentArm));
    m_angularForce = force - parallelComp;
    glm::vec2 torque = m_angularForce * (float)momentArm.length();
    m_angualrAcceleration = torque / m_moment;
    */
    
  //  m_angularVelocity += (force.y * pos.x - force.x * pos.y) / (m_moment);

}

void Rigidbody::resolveCollision(Rigidbody* actor2, glm::vec2 contact,glm::vec2* collisionNormal)
{
    // find the vector between their centres, or use the provided direction
    // of force, and make sure it's normalised
    glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal :
        actor2->m_position - m_position);
    // get the vector perpendicular to the collision normal
    glm::vec2 perp(normal.y, -normal.x);
    // detemine the total velocity of the contact points for the two objects,
    // for both linear and rotational
    // 'r' is the radius from axis to application of force
    float r1 = glm::dot(contact - m_position, -perp);
    float r2 = glm::dot(contact - actor2->m_position, perp);
    // velocity of the contact point on this object
    float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
    // velocity of contact point on actor2
    float v2 = glm::dot(actor2->m_velocity, normal) +
        r2 * actor2->m_angularVelocity;


        if (v1 > v2) // they're moving closer
        {
            // calculate the effective mass at contact point for each object
            // ie how much the contact point will move due to the force applied.
            float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
            float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2 * r2) / actor2->m_moment);
            float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;
            glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
                (mass1 + mass2) * (v1 - v2) * normal;
            //apply equal and opposite forces
            applyForce(-force, contact - m_position);
            actor2->applyForce(force, contact - actor2->m_position);
        }
}
float Rigidbody::getKineticEnergy()
{
    return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity) +
        m_moment * m_angularVelocity * m_angularVelocity);
}