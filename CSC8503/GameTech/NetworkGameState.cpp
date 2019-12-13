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
			*pushResult = (PushdownState*) new FinalScreenNetworkingState(3, game->getLeaderboard());
		} else {
			*pushResult = (PushdownState*) new FinalScreenNetworkingState(4, game->getLeaderboard());
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

