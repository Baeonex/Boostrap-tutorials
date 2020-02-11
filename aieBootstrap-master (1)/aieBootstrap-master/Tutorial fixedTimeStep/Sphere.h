#pragma once
#include "RigidBody.h"

class Sphere : public Rigidbody {
public: 
    Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
      virtual void makeGizmo(); 
      virtual bool checkCollision(PhysicsObject* pOther);

      void resolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal=nullptr)
      {
          // find the vector between their centres, or use the provided direction
          // of force, and make sure it's normalised
          glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal :
              actor2->getPosition() - m_position);
          // get the vector perpendicular to the collision normal
          glm::vec2 perp(normal.y, -normal.x);
          // detemine the total velocity of the contact points for the two objects,
          // for both linear and rotational
          // 'r' is the radius from axis to application of force
          float r1 = glm::dot(contact - m_position, -perp);
          float r2 = glm::dot(contact - actor2->getPosition(), perp);
          // velocity of the contact point on this object
          float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
          // velocity of contact point on actor2
          float v2 = glm::dot(actor2->getVelocity(), normal) +
              r2 * actor2->getAngualarVelocity();
          if (v1 > v2) // they're moving closer
          {
              // calculate the effective mass at contact point for each object
              // ie how much the contact point will move due to the force applied.
              float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
              float mass2 = 1.0f / (1.0f / actor2->getMass() + (r2 * r2) / actor2->getMoment());
              float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;
              glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
                  (mass1 + mass2) * (v1 - v2) * normal;
              //apply equal and opposite forces
              applyForce(-force, contact - m_position);
              actor2->applyForce(force, contact - actor2->getPosition());
          }
      }
      float getRadius() { return m_radius; }  glm::vec4 getColour() { return m_colour; }

protected:
    float m_radius;  glm::vec4 m_colour;
};
