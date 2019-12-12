#include "FinalScreenState.h"

NCL::CSC8503::FinalScreenState::FinalScreenState(int points)
{
	GameWorld* world = new GameWorld();
	GameTechRenderer* renderer = new GameTechRenderer(*world);
	renderer->setBackgroundColor(Vector4(0.56f, 0.72f, 0.65f, 1));

	this->finalMenu = new Menu(renderer);
	finalMenu->addChoice("Press Enter to continue...");
}

void NCL::CSC8503::FinalScreenState::OnAwake()
{
}

void NCL::CSC8503::FinalScreenState::OnSleep()
{
}

void NCL::CSC8503::FinalScreenState::Update()
{
	finalMenu->interact();
	finalMenu->displayMenu(points);
}

PushdownState::PushdownResult FinalScreenState::PushdownUpdate(PushdownState** pushResult)
{
	Update();
	if (finalMenu->isEnterPressed()) {
		return PushdownResult::Pop;
	}
	return PushdownState::NoChange;
}


