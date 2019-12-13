#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Common/MeshGeometry.h"
#include "PlayerObject.h"

using namespace NCL;
using namespace CSC8503;

class bridgeLog : public GameObject
{
public:
	bridgeLog(Vector3 position, Vector3 logSize, MeshGeometry* mesh, ShaderBase* shader, GameWorld* world);
	~bridgeLog() {};

	virtual void OnCollisionBegin(GameObject* otherObject) override {};
	virtual void OnCollisionEnd(GameObject* otherObject) override;
private:
	GameWorld* world;
};

