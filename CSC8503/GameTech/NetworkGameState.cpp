#include "NetworkGameState.h"

NetworkGameState::NetworkGameState(bool isServer)
{
	this->game = new NetworkedGame(isServer);
	this->isOnGoing = false;
	this->isServer = isServer;
}

void NCL::CSC8503::NetworkGameState::OnAwake()
{
	delete game;
	this->game = new NetworkedGame(isServer);
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
	} else if (game->getIsFinish()) {
		if (isServer) {
			*pushResult = (PushdownState*) new FinalScreenNetworkingState(20, game->getLeaderboard(), true);
		} else {
			*pushResult = (PushdownState*) new FinalScreenNetworkingState(21, game->getLeaderboard(), false);
		}
		return PushdownResult::Push;
	} else {
		return PushdownResult::NoChange;
	}
}

void NetworkGameState::initGame()
{
	game->InitCamera();
	game->InitWorld();
}

