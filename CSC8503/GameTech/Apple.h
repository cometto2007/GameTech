#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/SphereVolume.h"
#include "../../Common/MeshGeometry.h"
#include "PlayerObject.h"
#include "Loader.h"

using namespace NCL;
using namespace CSC8503;

class Apple : public GameObject
{
public:
	Apple(Vector3 position, MeshGeometry* mesh, ShaderBase* shader);
	~Apple() {};

	void followPlayer();

	virtual void OnCollisionBegin(GameObject* otherObject) override;
	virtual void OnCollisionEnd(GameObject* otherObject) override {};
private:
	Loader loader = Loader::getInstance();

	PlayerObject* player;
	NavigationGrid* navGrid;

	bool taken;
};

