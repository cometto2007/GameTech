#include "MenuState.h"

using namespace NCL::CSC8503;

MenuState::MenuState()
{
	GameWorld* world = new GameWorld();
	GameTechRenderer* renderer = new GameTechRenderer(*world);
	renderer->setBackgroundColor(Vector4(0.56f, 0.72f, 0.65f, 1));

	world->GetMainCamera()->SetNearPlane(0.5f);
	world->GetMainCamera()->SetFarPlane(2000.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(315.0f);
	world->GetMainCamera()->SetPosition(Vector3(-60, 40, 60));
	
	this->mainMenu = new Menu(renderer);
	mainMenu->addChoice("New Game");
	mainMenu->addChoice("Resume");
	mainMenu->addChoice("Scores");

	gamestate = new GameState();
}

void MenuState::OnAwake()
{
	
}

void MenuState::OnSleep()
{
	if (mainMenu->getSelChoice() == 0) {
		gamestate->initGame();
	}
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
		if (mainMenu->getSelChoice() == 0 || mainMenu->getSelChoice() == 1) {
			*pushResult = (PushdownState*)gamestate;
			return PushdownResult::Push;
		}
	}
	return PushdownResult::NoChange;
}



