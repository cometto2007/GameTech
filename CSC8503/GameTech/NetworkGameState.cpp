#include "NetworkGameState.h"

NetworkGameState::NetworkGameState()
{
	this->game = new NetworkedGame();
	this->isOnGoing = false;
}

void NetworkGameState::Update()
{
	float dt = Window::GetWindow()->GetTimer()->GetTimeDeltaSeconds();
	this->game->UpdateGame(dt);
}

PushdownState::PushdownResult NetworkGameState::PushdownUpdate(PushdownState** pushResult)
{
	Update();
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P)) {
		return PushdownResult::Pop;
	}
	else {
		return PushdownResult::NoChange;
	}
}

void NetworkGameState::initGame()
{
	game->InitCamera();
	game->InitWorld();
}

