#include "PhysicsScene.h"
#include <list>
#include "RigidBody.h"
#include "Sphere.h"
#include "PLane.h"
#include <iostream>
#include "Box.h"
using namespace std;

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0)) { }

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors) { delete pActor; }
}
void PhysicsScene::update(float dt) {

	// update physics at a fixed time step 
	static float accumulatedTime = 0.0f; 
	accumulatedTime += dt; 

	while (accumulatedTime >= m_timeStep) {
		for (auto pActor : m_actors) 
		{
			pActor->fixedUpdate(m_gravity, m_timeStep); 
		}  
		accumulatedTime -= m_timeStep;
		checkForCollision();
		// check for collisions (ideally you'd want to have some sort of    // scene management in place)   
		
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
	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere, PhysicsScene::plane2Box,
	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere, PhysicsScene::sphere2Box,
	PhysicsScene::box2Plane, PhysicsScene::box2Sphere, PhysicsScene::box2Box,


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
			// this check will ensure we don't include any joints 
			// in the collision checks
			if (shapeId1 < 0 || shapeId2 < 0)
				continue;
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
			glm::vec2 delta = sphere2->getPosition() - sphere1->getPosition();
			float distance = glm::length(delta);
			glm::vec2 contactForce = 0.5f * (distance - (sphere1->getRadius() +
				sphere2->getRadius())) * delta / distance;

			sphere1->setPosition(sphere1->getPosition() + contactForce);
			sphere2->setPosition(sphere2->getPosition() - contactForce);



			sphere1->resolveCollision(sphere2, 0.5f * (sphere1->getPosition() + sphere2->getPosition()));
			return true;
		}
		else return false;
		//this is where the collision detection happens
		//you need code which sets the velocity of the two spheres to zero
		//if they are overlapping
		// get distance between 2 spheres
		// if distance is less than the combined radius of
		// both spheres, then a collision occurred so set the
		// velocity of both spheres to 0 (we�ll add collision resolution later)
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
		glm::vec2 contact = sphere->getPosition() + (collisionNormal * -sphere->getRadius());
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
			sphere->setPosition(sphere->getPosition() + plane->getNormal() * (sphere->getRadius() - sphereToPlane));
			plane->resolveCollision(sphere, contact);
			//set sphere velocity to zero here
			return true;
		}
	}
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return sphere2Plane(obj2, obj1);
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

bool PhysicsScene::box2Plane(PhysicsObject* obj1, PhysicsObject* obj2) {
	Box* box = dynamic_cast<Box*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);
	//if we are successful then test for collision
	if (box != nullptr && plane != nullptr) {
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;
		float radius = 0.5f * std::fminf(box->getWidth(), box->getHeight());
		float penetration = 0;

		// which side is the centre of mass on?
		glm::vec2 planeOrigin = plane->getNormal() * plane->getDistance();
		float comFromPlane = glm::dot(box->getPosition() - planeOrigin,
			plane->getNormal());

		// check all four corners to see if we've hit the plane
		for (float x = -box->getExtents().x; x < box->getWidth(); x += box->getWidth()) {
			for (float y = -box->getExtents().y; y < box->getHeight(); y += box->getHeight()) {
				// get the position of the corner in world space
				glm::vec2 p = box->getPosition() + x * box->getLocalX() +
					y * box->getLocalY();
				float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());
				// this is the total velocity of the point
				float velocityIntoPlane = glm::dot(box->getVelocity() +
					box->getAngualarVelocity() * (-y * box->getLocalX() + x *
						box->getLocalY()), plane->getNormal());
				// if this corner is on the opposite side from the COM,
				// and moving further in, we need to resolve the collision
				if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane > 0) ||
					(distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane < 0)) {
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;

					if (comFromPlane >= 0) {
						if (penetration > distFromPlane)
							penetration = distFromPlane;
					}
					else {
						if (penetration < distFromPlane)
							penetration = distFromPlane;
					}
				}
			}
		}
		// we've had a hit - typically only two corners can contact
		if (numContacts > 0) {
			// get the average collision velocity into the plane
			// (covers linear and rotational velocity of all corners involved)
			float collisionV = contactV / (float)numContacts;
			// get the acceleration required to stop (restitution = 0) or reverse
			// (restitution = 1) the average velocity into the plane
			glm::vec2 acceleration = -plane->getNormal() *
				((1.0f + box->getElasticity()) * collisionV);
			// and the average position at which we'll apply the force
			// (corner or edge centre)
			glm::vec2 localContact = (contact / (float)numContacts) - box->getPosition();
			// this is the perpendicular distance we apply the force at relative to
			// the COM, so Torque = F*r
			float r = glm::dot(localContact, glm::vec2(plane->getNormal().y,
				-plane->getNormal().x));
			// work out the "effective mass" - this is a combination of moment of
			// inertia and mass, and tells us how much the contact point velocity
			// will change with the force we're applying
			float mass0 = 1.0f / (1.0f / box->getMass() + (r * r) / box->getMoment());
			// and apply the force
			box->setPosition(box->getPosition() - plane->getNormal() * penetration);
			box->applyForce(acceleration * mass0, localContact);
		}
	}
	return false;
}

bool PhysicsScene::box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2) {
	Box* box = dynamic_cast<Box*>(obj1);
	Sphere* sphere = dynamic_cast<Sphere*>(obj2);
	if (box != nullptr && sphere != nullptr) {
		glm::vec2 circlePos = sphere->getPosition() - box->getPosition();
		float w2 = box->getWidth() / 2, h2 = box->getHeight() / 2;
		int numContacts = 0;
		glm::vec2 contact(0, 0); // contact is in our box coordinates
		// check the four corners to see if any of them are inside the circle
		for (float x = -w2; x <= w2; x += box->getWidth()) {
			for (float y = -h2; y <= h2; y += box->getHeight()) {
				glm::vec2 p = x * box->getLocalX() + y * box->getLocalY();
				glm::vec2 dp = p - circlePos;
				if (dp.x * dp.x + dp.y * dp.y < sphere->getRadius() * sphere->getRadius()) {
					numContacts++;
					contact += glm::vec2(x, y);
				}
			}
		}
		glm::vec2* direction = nullptr;
		// get the local position of the circle centre
		glm::vec2 localPos(glm::dot(box->getLocalX(), circlePos),
			glm::dot(box->getLocalY(), circlePos));
		if (localPos.y < h2 && localPos.y > -h2) {
			if (localPos.x > 0 && localPos.x < w2 + sphere->getRadius()) {
				numContacts++;
				contact += glm::vec2(w2, localPos.y);
				direction = new glm::vec2(box->getLocalX());
			}
			if (localPos.x < 0 && localPos.x > -(w2 + sphere->getRadius())) {
				numContacts++;
				contact += glm::vec2(-w2, localPos.y);
				direction = new glm::vec2(-box->getLocalX());
			}
		}
		if (localPos.x < w2 && localPos.x > -w2) {
			if (localPos.y > 0 && localPos.y < h2 + sphere->getRadius()) {
				numContacts++;
				contact += glm::vec2(localPos.x, h2);
				direction = new glm::vec2(box->getLocalY());
			}
			if (localPos.y < 0 && localPos.y > -(h2 + sphere->getRadius())) {
				numContacts++;
				contact += glm::vec2(localPos.x, -h2);
				direction = new glm::vec2(-box->getLocalY());
			}
		}
		if (numContacts > 0) {

			contact = box->getPosition() + (1.0f / numContacts) * (box->getLocalX()
				* contact.x + box->getLocalY() * contact.y);
			// with the contact point we can find a penetration vector
			float pen = sphere->getRadius() - glm::length(contact - sphere->getPosition());
			glm::vec2 penVec = glm::normalize(contact - sphere->getPosition()) * pen;
			// move each shape away in the direction of penitration
			if (!box->isKinematic() && !sphere->isKinematic()) {
				box->setPosition(box->getPosition() + penVec * 0.5f);
				sphere->setPosition(sphere->getPosition() - penVec * 0.5f);
			}
			else if (!box->isKinematic()) {
				box->setPosition(box->getPosition() + penVec);
			}
			else {
				sphere->setPosition(sphere->getPosition() - penVec);
			}
			box->resolveCollision(sphere, contact, direction);
		}
		delete direction;
	}
	return false;
}


bool PhysicsScene::box2Box(PhysicsObject* obj1, PhysicsObject* obj2) {
	Box* box1 = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);
	if (box1 != nullptr && box2 != nullptr) {
		glm::vec2 boxPos = box2->getPosition() - box1->getPosition();
		glm::vec2 norm(0, 0);
		glm::vec2 contact(0, 0);
		float pen = 0;
		int numContacts = 0;
		box1->checkBoxCorners(*box2, contact, numContacts, pen, norm);
		if (box2->checkBoxCorners(*box1, contact, numContacts, pen, norm)) {
			norm = -norm;
		}
		if (pen > 0) {
			glm::vec2 penVec = glm::normalize(contact - box2->getPosition()) * pen;
			// move each shape away in the direction of penitration
			if (!box1->isKinematic() && !box2->isKinematic()) {
				box1->setPosition(box1->getPosition() + penVec * 0.5f);
				box2->setPosition(box2->getPosition() - penVec * 0.5f);
			}
			else if (!box1->isKinematic()) {
				box1->setPosition(box1->getPosition() + penVec);
			}
			else {
				box2->setPosition(box2->getPosition() - penVec);
			}

			box1->resolveCollision(box2, contact / float(numContacts), &norm);
		}
		return true;
	}
	return false;
}

bool PhysicsScene::plane2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return box2Plane(obj2, obj1);
}

bool PhysicsScene::sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return box2Sphere(obj2, obj1);
}