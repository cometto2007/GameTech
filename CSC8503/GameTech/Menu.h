#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "../../Common/Window.h"
#include "../../Plugins/OpenGLRendering/OGLRenderer.h"

using namespace NCL;

class Menu
{
public:
	Menu(OGLRenderer* renderer);
	~Menu() {};

	void addChoice(std::string choice);
	void displayMenu();
	void interact(KeyboardKeys k);

private:
	OGLRenderer* renderer;

	std::vector<std::string> choices;
	int selChoice;
};

