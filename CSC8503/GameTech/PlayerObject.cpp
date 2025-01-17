#include "PlayerObject.h"
#include "HumanEnemy.h"

using namespace NCL;

PlayerObject::PlayerObject(Vector3 position, MeshGeometry* mesh, ShaderBase* shader, bool isNetworkObj, bool isEnemy)
{
	float size = 1.0f;
	float inverseMass = 0.6f;
	this->isEnemy = isEnemy;

	SphereVolume* volume = new SphereVolume(size);
	boundingVolume = (CollisionVolume*)volume;

	transform.SetWorldScale(Vector3(size, size, size));
	transform.SetWorldPosition(position);

	renderObject = new RenderObject(&transform, mesh, nullptr, shader);
	physicsObject = new PhysicsObject(&transform, boundingVolume);

	physicsObject->SetInverseMass(inverseMass);
	physicsObject->InitSphereInertia();
	physicsObject->SetElasticity(0);

	if (isNetworkObj) {
		networkObject = new NetworkObject(*this, loader.getNewId());
	}
	if (isEnemy) {
		renderObject->SetColour(Vector4(1, 0, 0, 1));
	}
}

void PlayerObject::OnCollisionBegin(GameObject* otherObject)
{
	if (dynamic_cast<HumanEnemy*>(otherObject) != nullptr) {
		hitted = true;
		Vector3 axis(RandomFloat(-1, 1), 0, RandomFloat(-1, 1));
		physicsObject->AddForce(axis * moveForce * 50);
		physicsObject->AddTorque(Vector3(0,1,0) * moveForce);
	}
}

void PlayerObject::OnCollisionEnd(GameObject* otherObject)
{
}

void PlayerObject::move(Vector3 axis)
{
	axis.Normalise();
	physicsObject->AddForce(axis * moveForce);
}
