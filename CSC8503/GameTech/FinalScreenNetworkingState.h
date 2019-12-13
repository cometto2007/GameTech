#pragma once
#include "../CSC8503Common/PushdownState.h"
#include "GameState.h"
#include "Menu.h"

namespace NCL {
	namespace CSC8503 {
		class FinalScreenNetworkingState : public PushdownState
		{
		public:
			FinalScreenNetworkingState(int time, vector<int> leaderboard);
			~FinalScreenNetworkingState() {};

			virtual void OnAwake();
			virtual void OnSleep();

			virtual void Update();


			PushdownResult PushdownUpdate(PushdownState** pushResult) override;

		private:
			clock_t newStartTime = 0;
			int timerRespawn = 0;
			int remTime = 1;

			Menu* finalMenu;
			vector<int> leaderboard;
		};
	}
}



