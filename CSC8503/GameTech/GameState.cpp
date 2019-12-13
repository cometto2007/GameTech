#include "GameState.h"

NCL::CSC8503::GameState::GameState()
{
	this->game = new CourseworkGame();
	this->isOnGoing = false;
}

void NCL::CSC8503::GameState::Update()
{
	float dt = Window::GetWindow()->GetTimer()->GetTimeDeltaSeconds();
	this->game->UpdateGame(dt);
}

PushdownState::PushdownResult NCL::CSC8503::GameState::PushdownUpdate(PushdownState** pushResult)
{
	Update();
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P) || game->getRemainingTime() <= 0) {
		return PushdownResult::Pop;
	} else {
		return PushdownResult::NoChange;
	}
}

void GameState::initGame()
{
	game->InitCamera();
	game->InitWorld();
}
