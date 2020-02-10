#pragma once
#include "RigidBody.h"

class Sphere : public Rigidbody {
public: 
    Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
      virtual void makeGizmo(); 
      virtual bool checkCollision(PhysicsObject* pOther);
      virtual void resolveCollision(Rigidbody* actor2)
      {
          glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
          glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;
          float elasticity = 1;
          float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
              glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->getMass())));
          glm::vec2 force = normal * j;
          applyForceToActor(actor2, -force);
      }
      float getRadius() { return m_radius; }  glm::vec4 getColour() { return m_colour; }

protected:
    float m_radius;  glm::vec4 m_colour;
};
