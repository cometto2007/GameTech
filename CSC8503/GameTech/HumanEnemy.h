#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/PushdownMachine.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "Loader.h"
#include "CourseworkGame.h"

using namespace NCL;
using namespace CSC8503;

class HumanEnemy : public GameObject
{
public:
	HumanEnemy(Vector3 position, MeshGeometry* mesh, ShaderBase* shader, CourseworkGame* game, bool isParkKeeper = false);
	~HumanEnemy();

	void followPosition(Vector3 position);
	void shootBall(Vector3 position);

	PushdownMachine* getBehaviour() { return pushdownMachBehav; };
private:
	Loader loader = Loader::getInstance();

	PushdownMachine* pushdownMachBehav;
	NavigationGrid* navGrid;
	NavigationPath path;
	Vector3 currentGoalPos;
	void getRightGoalPos(Vector3 goalPos);

	float speed;
	bool isParkKeeper;
	CourseworkGame* game;
};

