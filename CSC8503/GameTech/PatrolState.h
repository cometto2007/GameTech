#pragma once
#include "HumanEnemy.h"
#include "GameState.h"
#include "../CSC8503Common/PushdownState.h"
#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8503 {
		class PatrolState : public PushdownState
		{
		public:
			PatrolState(HumanEnemy* enemy, PlayerObject* player, bool follow = false);
			~PatrolState();

			virtual void OnAwake();
			virtual void OnSleep();

			virtual void Update();

			PushdownResult PushdownUpdate(PushdownState** pushResult) override;
		private:
			HumanEnemy* enemy;
			PlayerObject* player;
			float patrolRadius;
			bool follow;

			vector<Vector3> patrolPos;
			int currentPatrolGoal;

			void setPatrolPositions(int numPos);
		};
	}
}

