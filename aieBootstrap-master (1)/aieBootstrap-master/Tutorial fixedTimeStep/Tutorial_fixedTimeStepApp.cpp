#include "Tutorial_fixedTimeStepApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "Sphere.h"
#include "PLane.h"
#include <cmath>
#include "Box.h"
#include <iostream>
#include "Button.h"
Tutorial_fixedTimeStepApp::Tutorial_fixedTimeStepApp() {

}

Tutorial_fixedTimeStepApp::~Tutorial_fixedTimeStepApp() {

}

bool Tutorial_fixedTimeStepApp::startup() {
	

	m_spawnBox = nullptr;
	m_spawnCircle = nullptr;
	m_2dRenderer = new aie::Renderer2D();
	m_windowWidth = aie::Application::getWindowWidth();
	m_windowHeight = aie::Application::getWindowHeight();
	// increase the 2d line count to maximize the number of objects we can draw  
	aie::Gizmos::create(16550U, 16550U, 65535U, 65535U);
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);
	m_physicsScene = new PhysicsScene;
	m_physicsScene->setGravity(glm::vec2(0, -10));
	m_physicsScene->setTimeStep(0.02f);

	m_plane = new Plane(glm::vec2(-1, 0), -70);//right
	Plane* m_plane4 = new Plane(glm::vec2(1, 0), -100);//left
	Plane* m_plane2 = new Plane(glm::vec2(0, -1), -56);//top
	Plane* m_plane3 = new Plane(glm::vec2(0, 1), -56);//bottom
	m_physicsScene->addActor(m_plane);
	Sphere * sphere = new Sphere(glm::vec2(-20, 50), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 1, 1));
	Sphere* sphere2 = new Sphere(glm::vec2(-50, 50), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 1, 1));
	Box* box = new Box(20.0f, 10.0f, 4.0f, glm::vec2(-50, 50), glm::vec4(1, 0, 0, 1), glm::vec2(0, 0));
	m_physicsScene->addActor(sphere);
	//m_physicsScene->addActor(sphere2);
	Box* box2 = new Box(20.0f, 10.0f, 4.0f, glm::vec2(-20, 50), glm::vec4(1, 0, 0, 1), glm::vec2(0, 0));
	m_physicsScene->addActor(box);
	//m_physicsScene->addActor(box2);
	m_physicsScene->addActor(m_plane);
	m_physicsScene->addActor(m_plane2);
	m_physicsScene->addActor(m_plane3);
	m_physicsScene->addActor(m_plane4);
	//sphere->applyForce(glm::vec2(30, 0), sphere->getPosition());
	box->setAngularVelocity(0.5f);
	box->applyForce(glm::vec2(-200, 0), box->getPosition());
	m_buttonBox = new Button("Box", 0.9, 0.9, 100, 50,glm::vec4(1,0,0,1));
	m_buttonCircle = new Button("Circle", 0.9, 0.8, 100, 50, glm::vec4(1, 0, 1, 1));
	//box2->applyForce(glm::vec2(-20, 0), box->getPosition());
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
	aie::Input* input = aie::Input::getInstance();
	m_windowWidth = aie::Application::getWindowWidth();
	m_windowHeight = aie::Application::getWindowHeight();
	// input example
	if (m_buttonBox->Update(m_windowHeight, m_windowWidth))
	{
		m_spawnBox = new Box(20.0f, 10.0f, 4.0f, glm::vec2(0, 0), glm::vec4(1, 0, 0, 1), glm::vec2(0, 0));
		m_spawnCircle = nullptr;
	}
	if (m_buttonCircle->Update(m_windowHeight, m_windowWidth))
	{
		m_spawnCircle = new Sphere(glm::vec2(0, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 1, 1));
		m_spawnBox = nullptr;
	}
	if (input->wasMouseButtonPressed(0) && input->getMouseX() < (m_windowWidth * 0.85))
	{
		//get the window dimensions
		glm::vec2 windowDimensions = glm::vec2(aie::Application::getWindowWidth() * 0.5, aie::Application::getWindowHeight() * 0.5);
		static float aspectRatio = windowDimensions.x / windowDimensions.y;

		//get mouse pos
		int mX, mY;
		input->getMouseXY(&mX, &mY);

		//convert mouse x and y to numbers in range -1 to 1;
		glm::vec2 normalizedMousePos = (glm::vec2((float)mX, (float)mY) / windowDimensions) - glm::vec2(1.0, 1.0);

		//convert mouse x and y into world space
		glm::vec2 worldMousePos = glm::vec2(normalizedMousePos.x * 100, normalizedMousePos.y * 100 / aspectRatio);

		if (m_spawnBox != nullptr)
		{
			m_spawnBox = new Box(20.0f, 10.0f, 4.0f, worldMousePos, glm::vec4(1, 0, 0, 1), glm::vec2(0, 0));
			m_physicsScene->addActor(m_spawnBox);
		}
		if (m_spawnCircle != nullptr)
		{
			m_spawnCircle = new Sphere(worldMousePos, glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 1, 1));
			m_physicsScene->addActor(m_spawnCircle);
		}
	}
	std::cout << input->getMouseX() << std::endl;
	aie::Gizmos::clear();
	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();
	m_plane->makeGizmo();
	std::cout << m_windowWidth << m_windowHeight << std::endl;
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
	m_buttonBox->DrawBox(m_2dRenderer,m_windowHeight,m_windowWidth);
	m_buttonCircle->DrawBox(m_2dRenderer, m_windowHeight, m_windowWidth);
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