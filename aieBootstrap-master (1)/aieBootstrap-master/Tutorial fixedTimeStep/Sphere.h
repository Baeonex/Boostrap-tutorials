#pragma once
#include "RigidBody.h"
#include <cmath>

class Sphere : public Rigidbody {
public: 
    Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
      virtual void makeGizmo(); 
      virtual float getMomentFormula() { return ((3.14f / 4) * std::pow(m_radius, 4)); }
      float getRadius() { return m_radius; }  glm::vec4 getColour() { return m_colour; }

protected:
    float m_radius;  glm::vec4 m_colour;
};
