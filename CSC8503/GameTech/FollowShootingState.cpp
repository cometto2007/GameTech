#include "FollowShootingState.h"

NCL::CSC8503::FollowShootingState::FollowShootingState(HumanEnemy* enemy, PlayerObject* player)
{
	this->enemy = enemy;
	this->player = player;
	maxChasingDist = 50.0f;
}

NCL::CSC8503::FollowShootingState::~FollowShootingState()
{
}

void NCL::CSC8503::FollowShootingState::OnAwake()
{
}

void NCL::CSC8503::FollowShootingState::OnSleep()
{
}

void NCL::CSC8503::FollowShootingState::Update()
{
	shootDelay++;
	enemy->followPosition(player->GetTransform().GetWorldPosition());
	if (shootDelay == 200) {
		enemy->shootBall(player->GetTransform().GetWorldPosition());
		shootDelay = 0;
	}
}

PushdownState::PushdownResult NCL::CSC8503::FollowShootingState::PushdownUpdate(PushdownState** pushResult)
{
	Update();
	float distance = (enemy->GetTransform().GetWorldPosition() - player->GetTransform().GetWorldPosition()).Length();
	if (distance < maxChasingDist) {
		return PushdownResult::NoChange;
	} else {
		return PushdownResult::Pop;
	}
}
