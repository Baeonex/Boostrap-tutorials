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
    }
    ~Rigidbody(){}

    virtual void fixedUpdate(glm::vec2 gravity, float timeStep);  
    virtual void debug();  
    void applyForce(glm::vec2 force); 
    void applyForceToActor(Rigidbody* actor2, glm::vec2 force);

    virtual void resolveCollision(Rigidbody* actor2);

    virtual bool checkCollision(PhysicsObject* pOther) = 0;

    glm::vec2 getPosition() { return m_position; } 
    float getRotation() { return m_rotation; }
    glm::vec2 getVelocity() { return m_velocity; }
    float getMass() { return m_mass; }
    glm::vec2 setVelocity(glm::vec2 newVelocity) { return m_velocity = newVelocity; }
protected: 
    glm::vec2 m_position; 
    glm::vec2 m_velocity;
    float m_mass;   
    float m_rotation; //2D so we only need a single float to represent our rotation
};
