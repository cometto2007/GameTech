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
	mainMenu->interact();
	mainMenu->displayMenu();
}

PushdownState::PushdownResult MenuState::PushdownUpdate(PushdownState** pushResult)
{
	Update();
	if (mainMenu->isEnterPressed()) {
		MenuState* ms = new MenuState(mainMenu);
		PushdownState* ps = ms;
		*pushResult = ps;
		return PushdownResult::Push;
	}
	else {
		return PushdownResult::NoChange;
	}
}



