#include "MenuState.h"

MenuState::MenuState(Menu* mainMenu)
{
	this->mainMenu = mainMenu;
}

void MenuState::OnAwake()
{
	
}

void MenuState::OnSleep()
{
}

void MenuState::Update()
{
	mainMenu->displayMenu();
}

PushdownState::PushdownResult NCL::CSC8503::MenuState::PushdownUpdate(PushdownState** pushResult)
{
	Update();
	return PushdownResult::NoChange;
}



