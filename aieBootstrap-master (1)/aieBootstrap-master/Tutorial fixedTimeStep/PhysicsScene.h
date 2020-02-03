#pragma once
#include "glm\ext.hpp"
#include <Vector>

enum ShapeType { PLANE = 0, SPHERE, BOX };

class PhysicsObject {

protected:  
    PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}
public:
    virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
    virtual void debug() = 0;  virtual void makeGizmo() = 0;
    virtual void resetPosition() {};

protected:
    ShapeType m_shapeID;
};

class PhysicsScene {
public:  
    PhysicsScene();  
    ~PhysicsScene();

    void addActor(PhysicsObject* actor); 
    void removeActor(PhysicsObject* actor); 
    void update(float dt);  
    void updateGizmos();
    void debugScene();
    void setGravity(const glm::vec2 gravity) { m_gravity = gravity; } 
    glm::vec2 getGravity() const { return m_gravity; }

    void setTimeStep(const float timeStep) { m_timeStep = timeStep; }  
    float getTimeStep() const { return m_timeStep; }

protected:  
    glm::vec2 m_gravity;  
    float m_timeStep;  
    std::vector<PhysicsObject*>  m_actors;
};
