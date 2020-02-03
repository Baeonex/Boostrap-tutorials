#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
class Tutorial_fixedTimeStepApp : public aie::Application {
public:

	Tutorial_fixedTimeStepApp();
	virtual ~Tutorial_fixedTimeStepApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	PhysicsScene* m_physicsScene;
};