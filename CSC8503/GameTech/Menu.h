#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "../../Plugins/OpenGLRendering/OGLRenderer.h"

using namespace NCL;

class Menu
{
public:
	Menu(OGLRenderer* renderer);
	~Menu() {};

	void addChoice(std::string choice);
	void displayMenu();
	void displayMenu(int points);
	void interact();
	int getSelChoice() { return selChoice; };

	bool isEnterPressed();

private:
	OGLRenderer* renderer;

	std::vector<std::string> choices;
	int selChoice;
	bool enterPressed;
	float delay;
};

