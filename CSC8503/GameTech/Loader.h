#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/NavigationGrid.h"

namespace NCL {
	namespace CSC8503 {
		class Loader
		{
		public:
			static Loader& getInstance()
			{
				static Loader instance;
				return instance;
			}
			void Destroy();

			NavigationGrid* getFloatingGrid() { return floatingGrid; };
			int getNewId() { 
				id++;
				return id - 1;
			};

		private:
			Loader();

			NavigationGrid* floatingGrid;
			int id = 10;
		};
	}
}
