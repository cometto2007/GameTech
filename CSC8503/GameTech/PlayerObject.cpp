#include "PlayerObject.h"

using namespace NCL;

PlayerObject::PlayerObject(Vector3 position, MeshGeometry* mesh, ShaderBase* shader, bool isNetworkObj, bool isEnemy)
{
	float size = 1.0f;
	float inverseMass = 0.6f;

	SphereVolume* volume = new SphereVolume(size);
	boundingVolume = (CollisionVolume*)volume;

	transform.SetWorldScale(Vector3(size, size, size));
	transform.SetWorldPosition(position);

	renderObject = new RenderObject(&transform, mesh, nullptr, shader);
	physicsObject = new PhysicsObject(&transform, boundingVolume);

	physicsObject->SetInverseMass(inverseMass);
	physicsObject->InitSphereInertia();

	if (isNetworkObj) {
		networkObject = new NetworkObject(*this, loader.getNewId());
	}
	if (isEnemy) {
		renderObject->SetColour(Vector4(1, 0, 0, 1));
	}
}

void PlayerObject::OnCollisionBegin(GameObject* otherObject)
{
}

void PlayerObject::OnCollisionEnd(GameObject* otherObject)
{
}

void PlayerObject::move(Vector3 axis)
{
	axis.Normalise();
	physicsObject->AddForce(axis * moveForce);
}
