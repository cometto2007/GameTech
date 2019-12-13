#include "Loader.h"

void Loader::Destroy()
{
	delete floatingGrid;
}

Loader::Loader()
{
	floatingGrid = new NavigationGrid("TestGrid1.txt");
	humanGrid = new NavigationGrid("TestGrid2.txt");
}
