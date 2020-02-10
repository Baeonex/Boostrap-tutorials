#include "PhysicsScene.h"
#include <list>
#include "RigidBody.h"
#include "Sphere.h"
#include "PLane.h"
#include <iostream>
const double SHAPE_COUNT = 2;
using namespace std;

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0)) { }

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors) { delete pActor; }
}
void PhysicsScene::update(float dt) {
	static std::list<PhysicsObject*> dirty;

	// update physics at a fixed time step 
	static float accumulatedTime = 0.0f; 
	accumulatedTime += dt; 

	while (accumulatedTime >= m_timeStep) {
		for (auto pActor : m_actors) 
		{
			pActor->fixedUpdate(m_gravity, m_timeStep); 
		}  
		accumulatedTime -= m_timeStep;

		// check for collisions (ideally you'd want to have some sort of    // scene management in place)   
		for (auto pActor : m_actors) {
			for (auto pOther : m_actors) {
				if (pActor == pOther)
					continue;
				if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() && std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
					continue;

				Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor);
				if (pRigid->checkCollision(pOther))
				{
					Rigidbody* pOtherRigid = dynamic_cast<Rigidbody*>(pOther);

					// P = m * v
					// f = P / t
					// f = m * a (constant mass)

					/* original code
					pRigid->ApplyForceToActor(
						pOtherRigid,
						pRigid->GetVelocity() * pRigid->GetMass()
					);
					*/
					float combinedMass = (pRigid->getMass() * pOtherRigid->getMass()) / (pRigid->getMass() + pOtherRigid->getMass()) * 3.0;
					glm::vec2 ImpactVector = glm::normalize(pOtherRigid->getPosition() - pRigid->getPosition());
					glm::vec2 relativeVelocity = pRigid->getVelocity() - pOtherRigid->getVelocity();
					//need to remove overlap at this point but I have not written the code to do this yet!
					float transmittedForce = glm::dot(relativeVelocity, ImpactVector);
					pRigid->applyForceToActor(
						pOtherRigid,
						ImpactVector * transmittedForce * combinedMass);
					dirty.push_back(pRigid);
					dirty.push_back(pOther);
				}
			}
		}
		dirty.clear();
	}
} 

void PhysicsScene::debugScene()
{
	int count = 0; 
	for (auto pActor : m_actors) 
	{
	cout << count << " : "; 
	pActor->debug();  
	count++; }
}


void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

void PhysicsScene::addActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject* actor)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (actor == m_actors[i])
		{
			m_actors.erase(m_actors.begin() + i);
		}
	}

	
}


typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	 PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere,
	 PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere,
};


void PhysicsScene::checkForCollision()
{
	int actorCount = m_actors.size();
	//need to check for collisions against all objects except this one.
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->getShapeID();
			int shapeId2 = object2->getShapeID();
			// using function pointers
			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				// did a collision occur?
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}
bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);
	//if we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		if (glm::distance(sphere1->getPosition(), sphere2->getPosition()) < (sphere1->getRadius() + sphere2->getRadius()))
		{
			sphere1->resolveCollision(sphere2);
			return true;
		}
		else return false;
		//this is where the collision detection happens
		//you need code which sets the velocity of the two spheres to zero
		//if they are overlapping
		// get distance between 2 spheres
		// if distance is less than the combined radius of
		// both spheres, then a collision occurred so set the
		// velocity of both spheres to 0 (we’ll add collision resolution later)
	}
	return false;
}

bool PhysicsScene::sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);
	//if we are successful then test for collision
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(
			sphere->getPosition(),
			plane->getNormal()) - plane->getDistance();
		// if we are behind plane then we flip the normal
		if (sphereToPlane < 0) {
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}
		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0) {
			plane->resolveCollision(sphere);
			//set sphere velocity to zero here
			return true;
		}
		std::cout << "it workd" << std::endl;
	}
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);
	//if we are successful then test for collision
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(
			sphere->getPosition(),
			plane->getNormal()) - plane->getDistance();
		// if we are behind plane then we flip the normal
		if (sphereToPlane < 0) {
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}
		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0) {
			//set sphere velocity to zero here
			return true;
		}
		std::cout << "it workd" << std::endl;
	}
	return false;
}
bool PhysicsScene::plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);
	//if we are successful then test for collision
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(
			sphere->getPosition(),
			plane->getNormal()) - plane->getDistance();
		// if we are behind plane then we flip the normal
		if (sphereToPlane < 0) {
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}
		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0) {
			//set sphere velocity to zero here
			return true;
		}
	}
	return false;
}