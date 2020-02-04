#pragma once
#include "PhysicsScene.h"
class Plane : public PhysicsObject
{
public:
    Plane() : PhysicsObject(ShapeType::PLANE) {
        m_distanceToOrigin = 0;
        m_normal = glm::vec2(0, 1);
    }
    Plane(glm::vec2 normal, float distance) : PhysicsObject(ShapeType::PLANE)
    {
        m_distanceToOrigin = distance;
        m_normal = normal;
    }
    ~Plane()
    {

    }

    virtual void fixedUpdate(glm::vec2 gravity, float timeStep) {}
    virtual void debug() {}
    virtual void makeGizmo();
    virtual void resetPosition() {
        m_distanceToOrigin = 0;  
        m_normal = glm::vec2(0, 1);
    }
    glm::vec2 getNormal() { return m_normal; }
    float getDistance() { return m_distanceToOrigin; }

protected:  
    glm::vec2 m_normal; 
    float m_distanceToOrigin;
};

