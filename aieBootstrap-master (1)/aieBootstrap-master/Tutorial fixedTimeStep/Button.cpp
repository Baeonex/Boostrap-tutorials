#include "Button.h"
#include <string>
#include "Gizmos.h"

Button::Button(const char* buttonText, float x, float y, float width, float height, glm::vec4 colour)
{
	//Store the text that will appear on the button.
	strcpy_s(m_buttonText, 64, buttonText);
	//Load a font to use to display the text.
	m_font = new aie::Font("./font/consolas.ttf", 24);

	//Store the button's position, width, and height.
	m_posX = x;
	m_posY = y;
	m_width = width;
	m_height = height;
	m_colour = colour;
}

Button::~Button()
{
	delete m_font;
}


bool Button::Update(float height, float width)
{

	aie::Input* input = aie::Input::getInstance();
	//Get mouse position
	int mouseX = input->getMouseX();
	int mouseY = input->getMouseY();

	//Calculate the 4 sides of the button

	glm::vec2 windowDimensions = glm::vec2(width, height);
	static float aspectRatio = windowDimensions.x / windowDimensions.y;
	glm::vec2 worldPos = glm::vec2(width * m_posX, height * m_posY);

	float left = worldPos.x - (m_width * 0.5f);
	float right = worldPos.x + (m_width * 0.5f);
	float bottom = worldPos.y - (m_height * 0.5f);
	float top = worldPos.y + (m_height * 0.5f);
	//Check if the mouse is inside the box made by the 4 sides.
	if (mouseX > left&& mouseX < right &&
		mouseY > bottom&& mouseY < top)
	{
		//Return whether the mouse button is clicked while colliding
		return input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT);
	}
	return false;


}

void Button::DrawBox(aie::Renderer2D* renderer, float height, float width)
{
	renderer->setRenderColour(m_colour.x, m_colour.y, m_colour.z, m_colour.w);
	glm::vec2 windowDimensions = glm::vec2(width, height);
	static float aspectRatio = windowDimensions.x / windowDimensions.y;
	glm::vec2 worldPos = glm::vec2(width * m_posX,height * m_posY);
	renderer->drawBox(worldPos.x, worldPos.y, m_width, m_height);
	renderer->setRenderColour(1, 1, 1, 1);
	//Calculating the centred text position is a bit fiddly.
	float textWidth = m_font->getStringWidth(m_buttonText);
	float textHeight = m_font->getStringHeight(m_buttonText);
	float centredPosX = worldPos.x - (textWidth * 0.5f) + 2;
	float centredPosY = worldPos.y - (textHeight * 0.5f) + 5;
	//Draw text on the button.
	renderer->drawText(m_font, m_buttonText, centredPosX, centredPosY);

}
