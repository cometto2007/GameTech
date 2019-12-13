#pragma once

#include "../../Plugins/OpenGLRendering/OGLRenderer.h"
#include <vector>
#include <string>
#include <algorithm>


using namespace NCL;

class Menu
{
public:
	Menu(OGLRenderer* renderer);
	~Menu() {};

	void addChoice(std::string choice);
	void displayMenu();
	void displayMenu(int points);
	void displayMenu(std::vector<int> leaderboards, int time);
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

