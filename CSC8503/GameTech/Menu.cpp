#include "Menu.h"

Menu::Menu(OGLRenderer* renderer)
{
	delay = 0.0f;
	selChoice = 0;
	enterPressed = false;
	this->renderer = renderer;
}

void Menu::addChoice(std::string choice)
{
	choices.push_back(choice);
}

void Menu::displayMenu()
{
	float screenY = Window::GetWindow()->GetScreenSize().y / 2;
	renderer->Render();
	for (size_t i = 0; i < choices.size(); i++) {
		if (i == selChoice) {
			renderer->DrawString(choices[i], Vector2(20, screenY - i * 25), Vector4(1, 0, 0, 1));
		} else {
			renderer->DrawString(choices[i], Vector2(20, screenY - i * 25), Vector4(1, 1, 1, 1));
		}	
	}
}

void Menu::displayMenu(int points)
{
	float screenY = Window::GetWindow()->GetScreenSize().y / 2;
	renderer->Render();

	renderer->DrawString("Your final score is " + std::to_string(points), Vector2(20, screenY), Vector4(1, 0, 0, 1));
	renderer->DrawString(choices[0], Vector2(20, 10), Vector4(1, 0, 0, 1));
}

void Menu::interact()
{
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::DOWN)) {
			selChoice = min(selChoice + 1, (int)choices.size() - 1);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::UP)) {
			selChoice = max(0, selChoice - 1);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::RETURN)) {
			enterPressed = true;
		}
}

bool Menu::isEnterPressed()
{
	if (enterPressed) {
		enterPressed = !enterPressed;
		return !enterPressed;
	}
	return false;
	
}
