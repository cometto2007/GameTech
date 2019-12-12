#include "NetworkGameState.h"

NetworkGameState::NetworkGameState(bool isServer)
{
	this->game = new NetworkedGame(isServer);
	this->isOnGoing = false;
	this->isServer = isServer;
}

void NetworkGameState::Update()
{
	float dt = Window::GetWindow()->GetTimer()->GetTimeDeltaSeconds();
	std::cout << dt << std::endl;
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

