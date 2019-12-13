#include "FinalScreenNetworkingState.h"

NCL::CSC8503::FinalScreenNetworkingState::FinalScreenNetworkingState(int time, vector<int> leaderboard)
{
	GameWorld* world = new GameWorld();
	GameTechRenderer* renderer = new GameTechRenderer(*world);
	renderer->setBackgroundColor(Vector4(0.56f, 0.72f, 0.65f, 1));

	newStartTime = clock();
	timerRespawn = time;

	this->finalMenu = new Menu(renderer);
	this->leaderboard = leaderboard;
}

void NCL::CSC8503::FinalScreenNetworkingState::OnAwake()
{
}

void NCL::CSC8503::FinalScreenNetworkingState::OnSleep()
{
}

void NCL::CSC8503::FinalScreenNetworkingState::Update()
{
	double secondsPassed = (clock() - newStartTime) / CLOCKS_PER_SEC;
	remTime = (int)(timerRespawn - secondsPassed);
	finalMenu->interact();
	finalMenu->displayMenu(leaderboard, remTime);
}

PushdownState::PushdownResult FinalScreenNetworkingState::PushdownUpdate(PushdownState** pushResult)
{
	Update();
	if (remTime <= 0) {
		return PushdownResult::Pop;
	}
	return PushdownState::NoChange;
}


