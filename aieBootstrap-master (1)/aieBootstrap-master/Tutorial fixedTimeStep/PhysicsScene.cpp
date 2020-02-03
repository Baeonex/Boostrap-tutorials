#include "PhysicsScene.h"

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0)) { }
PhysicsScene::~PhysicsScene()
{

}
void PhysicsScene::update(float dt)
{
	static float accumulatedTIme = 0.0f;
	accumulatedTIme += dt;
	while (accumulatedTIme >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTIme -= m_timeStep;
	}

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
	for (int i = 0; i > m_actors.size(); i++)
	{
		if (actor == m_actors[i])
		{
			m_actors.erase(m_actors.begin() + i);
		}
	}

	
}