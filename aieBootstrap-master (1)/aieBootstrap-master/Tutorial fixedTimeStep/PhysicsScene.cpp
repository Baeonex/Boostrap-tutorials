#include "PhysicsScene.h"
#include <list>
#include "RigidBody.h"
#include <iostream>
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

