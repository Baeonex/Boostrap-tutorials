#pragma once
#include "PhysicsScene.h"
#include "RigidBody.h"
class Plane : public Rigidbody
{
public:

    Plane() : Rigidbody(ShapeType::PLANE,glm::vec2(0,0), glm::vec2(0,0), 0,0,0) {
        m_distanceToOrigin = 0;
        m_normal = glm::vec2(0, 1);
    }
    Plane(glm::vec2 normal, float distance) : Rigidbody(ShapeType::PLANE, glm::vec2(0, 0), glm::vec2(0, 0), 0, 0,0)
    {
        m_distanceToOrigin = distance;
        m_normal = normal;
    }
    ~Plane()
    {

    }
    virtual void resolveCollision(Rigidbody* actor2, glm::vec2 contact);
    virtual void fixedUpdate(glm::vec2 gravity, float timeStep) {}
    virtual void debug() {}
    virtual void makeGizmo();
    virtual void resetPosition() {
        m_distanceToOrigin = 0;  
        m_normal = glm::vec2(0, 1);
    }
    virtual bool ContainsPoint(glm::vec2 point) const;
    virtual float getMomentFormula() { return 0; }
    glm::vec2 getNormal() { return m_normal; }
    float getDistance() { return m_distanceToOrigin; }

protected:  
    glm::vec2 m_normal; 
    float m_distanceToOrigin;
};

