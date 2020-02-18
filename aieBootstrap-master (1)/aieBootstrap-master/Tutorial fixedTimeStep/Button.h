#pragma once
#include "Renderer2D.h"
#include "Font.h"
#include "Application.h"
#include "Renderer2D.h"
#include "Input.h"
#include "Texture.h"
#include "glm\ext.hpp"
class Button
{
public:
	Button(const char* buttonText, float x, float y, float width, float height, glm::vec4 colour);
	~Button();
	void DrawBox(aie::Renderer2D* renderer, float height, float width);
	bool Update(float height, float width);

private:
	//Font and text for the button.
	aie::Font* m_font;
	char m_buttonText[64];
	//Button's position, width, and height.
	float m_posX;
	float m_posY;
	float m_width;
	float m_height;
	glm::vec4 m_colour;
};
