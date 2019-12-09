#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/SphereVolume.h"
#include "../../Common/MeshGeometry.h"
#include "Water.h"

using namespace NCL;
using namespace CSC8503;

class PlayerObject : public GameObject
{
public:
	PlayerObject(Vector3 position, MeshGeometry* mesh, ShaderBase* shader);
	~PlayerObject() {};

	virtual void OnCollisionBegin(GameObject* otherObject) override;
	virtual void OnCollisionEnd(GameObject* otherObject) override;

	float getMoveForce() { return moveForce; };
	void move(Vector3 axis);

private:
	const float moveForce = 100.0f;
};
