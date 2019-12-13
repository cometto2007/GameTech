#pragma once
#include "HumanEnemy.h"
#include "GameState.h"
#include "../CSC8503Common/PushdownState.h"
#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8503 {
		class FollowShootingState : public PushdownState
		{
		public:
			FollowShootingState(HumanEnemy* enemy, PlayerObject* player, float maxChasingDist = 50.0f);
			~FollowShootingState();

			virtual void OnAwake();
			virtual void OnSleep();

			virtual void Update();

			PushdownResult PushdownUpdate(PushdownState** pushResult) override;
		private:
			PlayerObject* player;
			HumanEnemy* enemy;
			float maxChasingDist;

			int shootDelay;
		};
	}
}
