#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/SphereVolume.h"
#include "../../Common/MeshGeometry.h"
#include "PlayerObject.h"

using namespace NCL;
using namespace CSC8503;

class Water : public GameObject
{
public:
	Water(Vector3 position, Vector3 waterSize, MeshGeometry* mesh, ShaderBase* shader);
	~Water() {};

	virtual void OnCollisionBegin(GameObject* otherObject) override {};
	virtual void OnCollisionEnd(GameObject* otherObject) override {};
};

