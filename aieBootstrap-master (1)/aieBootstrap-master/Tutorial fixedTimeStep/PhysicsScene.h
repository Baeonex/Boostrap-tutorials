#pragma once
#include "glm\ext.hpp"
#include <Vector>
enum ShapeType { JOINT = -1,PLANE = 0, SPHERE, BOX, SHAPE_COUNT };


class PhysicsObject {

protected:  
    PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}
public:
    virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
    virtual void debug() = 0;  virtual void makeGizmo() = 0;
    virtual void resetPosition() {};
    virtual bool ContainsPoint(glm::vec2 point) const = 0;
    ShapeType getShapeID() { return m_shapeID; }
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
    int getActorCount() { return m_actors.size(); }
    void setTimeStep(const float timeStep) { m_timeStep = timeStep; }  
    float getTimeStep() const { return m_timeStep; }
    std::vector<PhysicsObject*> getActors() { return m_actors; }
    void checkForCollision();
    static bool plane2Plane(PhysicsObject*, PhysicsObject*);
    static bool plane2Sphere(PhysicsObject*, PhysicsObject*);
    static bool sphere2Plane(PhysicsObject*, PhysicsObject*);
    static bool sphere2Sphere(PhysicsObject*, PhysicsObject*);
    static bool box2Plane(PhysicsObject*, PhysicsObject*);
    static bool box2Sphere(PhysicsObject*, PhysicsObject*);
    static bool box2Box(PhysicsObject*, PhysicsObject*);
    static bool plane2Box(PhysicsObject*, PhysicsObject*);
    static bool sphere2Box(PhysicsObject*, PhysicsObject*);
    
protected:  
    glm::vec2 m_gravity;  
    float m_timeStep;  
    std::vector<PhysicsObject*>  m_actors;
};
