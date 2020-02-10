#pragma once
#include "PhysicsScene.h"

class Rigidbody : public PhysicsObject
{
public:
    Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass) : PhysicsObject(shapeID) {
        m_shapeID = shapeID;
        m_position = position;
        m_velocity = velocity;
        m_rotation = rotation;
        m_mass = mass;
        m_angularDrag = 0.1f;
        m_elasticity = 1;
        m_linearDrag = 0.3f;
        m_isKinematic = false;
    }
    ~Rigidbody(){}

    virtual void fixedUpdate(glm::vec2 gravity, float timeStep);  
    virtual void debug();  
    void applyForce(glm::vec2 force, glm::vec2 pos); 

    virtual void resolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal=nullptr);

    virtual bool checkCollision(PhysicsObject* pOther) = 0;

    glm::vec2 getPosition() { return m_position; } 
    float getRotation() { return m_rotation; }
    glm::vec2 getVelocity() { return m_velocity; }
    float getMass() { return m_mass; }
    glm::vec2 setVelocity(glm::vec2 newVelocity) { return m_velocity = newVelocity; }

    float getLinearDrag() { return m_linearDrag; }
    float getAngularDrag() { return m_angularDrag; }
    float getElasticity() { return m_elasticity; }
    void setLinearDrag(float linearDrag) { m_linearDrag = linearDrag; }
    void setAngularDrag(float angularDrag) { m_angularDrag = angularDrag; }
    void setElasticity(float elasticity) { m_elasticity = elasticity; }
protected: 
    float m_linearDrag;
    float m_angularDrag;
    glm::vec2 m_position; 
    glm::vec2 m_velocity;
    float m_mass;   
    float m_rotation; //2D so we only need a single float to represent our rotation
    float m_angularVelocity;
    float m_moment;
    float m_elasticity;
    bool m_isKinematic;
};
