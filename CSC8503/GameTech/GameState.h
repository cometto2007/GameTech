#pragma once
#include "CourseworkGame.h"
#include "../CSC8503Common/PushdownState.h"

using namespace NCL::CSC8503;

namespace NCL {
	namespace CSC8503 {
		class GameState : public PushdownState
		{
		public:
			GameState();
			~GameState() {};

			virtual void OnAwake() {};
			virtual void OnSleep() {};

			virtual void Update();

			PushdownResult PushdownUpdate(PushdownState** pushResult) override;
			bool getIsOnGoing() { return isOnGoing; };
			CourseworkGame* getGame() { return game; };
			void initGame();
		private:
			CourseworkGame* game;
			bool isOnGoing;
		};
	}
}
