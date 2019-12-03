#include "Menu.h"

Menu::Menu(OGLRenderer* renderer)
{
	selChoice = 0;
	this->renderer = renderer;
}

void Menu::addChoice(std::string choice)
{
	choices.push_back(choice);
}

void Menu::displayMenu()
{
	float screenY = Window::GetWindow()->GetScreenSize().y / 2;
	for (size_t i = 0; i < choices.size(); i++) {
		if (i == selChoice) {
			renderer->DrawString(choices[i], Vector2(20, screenY - i * 25), Vector4(1, 0, 0, 1));
		} else {
			renderer->DrawString(choices[i], Vector2(20, screenY - i * 25), Vector4(1, 1, 1, 1));
		}	
	}
}

void Menu::interact(KeyboardKeys k)
{
	if (k == KeyboardKeys::DOWN) {
		selChoice = min(selChoice + 1, (int)choices.size() - 1);
	}
	if (k == KeyboardKeys::UP) {
		selChoice = max(0, selChoice - 1);
	}
	if (k == KeyboardKeys::RETURN) {
		// TODO
	}
}
