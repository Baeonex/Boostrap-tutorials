#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "PLane.h"
#include "Button.h"
class Tutorial_fixedTimeStepApp : public aie::Application {
public:

	Tutorial_fixedTimeStepApp();
	virtual ~Tutorial_fixedTimeStepApp();

	virtual bool startup();
	virtual void shutdown();
	void setupConinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity);
	virtual void update(float deltaTime);
	virtual void draw();
	glm::vec2 ToWorldSpace(int screenX, int screenY) const;

protected:
	Plane* m_plane;
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	PhysicsScene* m_physicsScene;
	int m_windowHeight, m_windowWidth;
	Button* m_buttonBox;
	Button* m_buttonCircle;
	Button* m_buttonGravityOFF;
	Button* m_buttonGravityON;
	std::vector<PhysicsObject*> m_springChain;
	bool m_gravityBool;
	Rigidbody* m_spawnBox;
	Rigidbody* m_spawnCircle;

};