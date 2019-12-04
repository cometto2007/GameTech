#pragma once
#include "../CSC8503Common/PushdownState.h"
#include "../../Plugins/OpenGLRendering/OGLRenderer.h"
#include "Menu.h"

using namespace NCL::CSC8503;

namespace NCL {
	namespace CSC8503 {
		class MenuState : public PushdownState
		{
		public:
			MenuState(Menu* mainMenu);
			~MenuState() {};

			virtual void OnAwake();
			virtual void OnSleep();

			virtual void Update();

			PushdownResult PushdownUpdate(PushdownState** pushResult) override;

			Menu* getMainMenu() { return mainMenu; };
		private:
			Menu* mainMenu;
		};
	}
}
