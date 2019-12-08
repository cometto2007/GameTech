#define _USE_MATH_DEFINES

#include "PatrolState.h"
#include <cmath>

NCL::CSC8503::PatrolState::PatrolState(HumanEnemy* enemy)
{
	this->enemy = enemy;
	patrolRadius = 25.0f;
	setPatrolPositions(10);
	currentPatrolGoal = 0;
}

NCL::CSC8503::PatrolState::~PatrolState()
{
}

void NCL::CSC8503::PatrolState::OnAwake()
{
}

void NCL::CSC8503::PatrolState::OnSleep()
{
}

void NCL::CSC8503::PatrolState::Update()
{
	float distance = (enemy->GetTransform().GetWorldPosition() - patrolPos[currentPatrolGoal]).Length();
	if (distance < 20.0f) {
		currentPatrolGoal++;
		if (currentPatrolGoal >= patrolPos.size()) {
			currentPatrolGoal = 0;
		}
	}
	enemy->followPosition(patrolPos[currentPatrolGoal]);
}

PushdownState::PushdownResult NCL::CSC8503::PatrolState::PushdownUpdate(PushdownState** pushResult)
{
	Update();
	return PushdownResult::NoChange;
}

void NCL::CSC8503::PatrolState::setPatrolPositions(int numPos)
{
	Vector3 enemyPos = enemy->GetTransform().GetWorldPosition();
	float x, z;
	for (int i = 0; i < numPos; i++) {
		float angle = (rand() % 360) * (M_PI / 180);
		x = patrolRadius * std::cos(angle);
		z = patrolRadius * std::sin(angle);
		patrolPos.push_back(Vector3(enemyPos.x + x, enemyPos.y, enemyPos.z + z));
	}
}
