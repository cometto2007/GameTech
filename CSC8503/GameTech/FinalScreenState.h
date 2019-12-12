#pragma once
#include "../CSC8503Common/PushdownState.h"
#include "GameState.h"
#include "Menu.h"

namespace NCL {
	namespace CSC8503 {
		class FinalScreenState : public PushdownState
		{
		public:
			FinalScreenState(int points);
			~FinalScreenState() {};

			virtual void OnAwake();
			virtual void OnSleep();

			virtual void Update();


			PushdownResult PushdownUpdate(PushdownState** pushResult) override;

		private:
			int points;
			Menu* finalMenu;
		};
	}
}

