#include "Tutorial_fixedTimeStepApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
Tutorial_fixedTimeStepApp::Tutorial_fixedTimeStepApp() {

}

Tutorial_fixedTimeStepApp::~Tutorial_fixedTimeStepApp() {

}

bool Tutorial_fixedTimeStepApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	// increase the 2d line count to maximize the number of objects we can draw  
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene;
	m_physicsScene->setTimeStep(0.01f);
	

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

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Tutorial_fixedTimeStepApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}