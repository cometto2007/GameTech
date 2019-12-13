#pragma once
#include "GameState.h"
#include "../CSC8503Common/PushdownState.h"
#include "Menu.h"
#include "NetworkGameState.h"
#include "FinalScreenState.h"
#include "FinalScreenNetworkingState.h"


namespace NCL {
	namespace CSC8503 {
		class MenuState : public PushdownState
		{
		public:
			MenuState();
			~MenuState() {};

			virtual void OnAwake();
			virtual void OnSleep();

			virtual void Update();

			PushdownResult PushdownUpdate(PushdownState** pushResult) override;

			Menu* getMainMenu() { return mainMenu; };
		private:
			Menu* mainMenu;
			GameState* gamestate;
			NetworkGameState* netGgamestate;
		};
	}
}
