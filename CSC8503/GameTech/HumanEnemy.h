#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/PushdownMachine.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "Loader.h"

using namespace NCL;
using namespace CSC8503;

class HumanEnemy : public GameObject
{
public:
	HumanEnemy(Vector3 position, MeshGeometry* mesh, ShaderBase* shader);
	~HumanEnemy();

	void followPosition(Vector3 position);
	PushdownMachine* getBehaviour() { return pushdownMachBehav; };
private:
	Loader loader = Loader::getInstance();

	PushdownMachine* pushdownMachBehav;
	NavigationGrid* navGrid;
	NavigationPath path;
	Vector3 currentGoalPos;

	float speed;
};

