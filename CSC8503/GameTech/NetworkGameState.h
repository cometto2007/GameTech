#pragma once
#include "NetworkedGame.h"
#include "../CSC8503Common/PushdownState.h"

using namespace NCL::CSC8503;

namespace NCL {
	namespace CSC8503 {
		class NetworkGameState : public PushdownState
		{
		public:
			NetworkGameState();
			~NetworkGameState() {};

			virtual void OnAwake() {};
			virtual void OnSleep() {};

			virtual void Update();

			PushdownResult PushdownUpdate(PushdownState** pushResult) override;
			bool getIsOnGoing() { return isOnGoing; };
			void initGame();
		private:
			NetworkedGame* game;
			bool isOnGoing;
		};
	}
}
