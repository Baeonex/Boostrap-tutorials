#include "Tutorial_fixedTimeStepApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "Sphere.h"
#include "PLane.h"
#include <cmath>
Tutorial_fixedTimeStepApp::Tutorial_fixedTimeStepApp() {

}

Tutorial_fixedTimeStepApp::~Tutorial_fixedTimeStepApp() {

}

bool Tutorial_fixedTimeStepApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	// increase the 2d line count to maximize the number of objects we can draw  
	aie::Gizmos::create(16550U, 16550U, 65535U, 65535U);

	m_font = new aie::Font("../bin/font/consolas.ttf", 32);
	m_physicsScene = new PhysicsScene;
	//m_physicsScene->setGravity(glm::vec2(0, -10));
	m_physicsScene->setTimeStep(0.02f);

	m_plane = new Plane(glm::vec2(1, 0), 1);
	Sphere * sphere = new Sphere(glm::vec2(-20, 50), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));

	m_physicsScene->addActor(sphere);
	m_physicsScene->addActor(m_plane);
	sphere->applyForce(glm::vec2(20, 0));

	/*
	m_physicsScene->setGravity(glm::vec2(0,-10));
	m_physicsScene->setTimeStep(0.5f);
	float radius = 1.0f;
	float speed = 30;
	glm::vec2 startPos(-40, 0);
	glm::vec2 incl(45, 45);
	float inclination = 3.14f / 4.0f;

	m_physicsScene->addActor(new Sphere(startPos,incl, speed,radius,glm::vec4(1,0,0,1)));

	setupConinuousDemo(glm::vec2(-40, 0), 45, 30, 10);

	*/

	//Sphere* ball1 = new Sphere(glm::vec2(-20, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1)); 
	//Sphere* ball2 = new Sphere(glm::vec2(10, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	//m_physicsScene->addActor(ball1);  m_physicsScene->addActor(ball2);

	//ball1->applyForce(glm::vec2(60, 30));  
	//ball2->applyForce(glm::vec2(-80, 30));
	

	return true;
}

void Tutorial_fixedTimeStepApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void Tutorial_fixedTimeStepApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();
	aie::Gizmos::clear();
	m_physicsScene->checkForCollision();
	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();
	m_plane->makeGizmo();
	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Tutorial_fixedTimeStepApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();
	//setupConinuousDemo(glm::vec2(40, 0), 40, 45, 10);
	// draw your stuff here!
	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void Tutorial_fixedTimeStepApp::setupConinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity) {
	float t = 0;  float tStep = 0.1f;  float radius = 1.0f;  int segments = 12;  glm::vec4 colour = glm::vec4(1, 1, 0, 1);

	while (t <= 5) {
		float x, y;
		float a;


		//  horizontal velocity = v * sin(inclination)           xPos = (horizontal veloctiy) * t + xInitial
		//	vertical velocity = yPos = -gt2 + (vertical velocity) * t + yInitial
		x = ((speed * sin(inclination / 57.29f)) * t + startPos.x);
		y = -(gravity / 2) * (t * t) + (speed * cos(inclination/57.29f)) * t + startPos.y;
	


		aie::Gizmos::add2DCircle(glm::vec2(x, y), radius, segments, colour); 
		t += tStep;
	}
}