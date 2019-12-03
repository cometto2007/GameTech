#include "PlayerObject.h"

using namespace NCL;

PlayerObject::PlayerObject(Vector3 position, MeshGeometry* mesh, ShaderBase* shader)
{
	float size = 1.0f;
	float inverseMass = 1.0f;

	SphereVolume* volume = new SphereVolume(size);
	boundingVolume = (CollisionVolume*)volume;

	transform.SetWorldScale(Vector3(size, size, size));
	transform.SetWorldPosition(position);

	renderObject = new RenderObject(&transform, mesh, nullptr, shader);
	physicsObject = new PhysicsObject(&transform, boundingVolume);

	physicsObject->SetInverseMass(inverseMass);
	physicsObject->InitSphereInertia();
}

void PlayerObject::OnCollisionBegin(GameObject* otherObject)
{
	/*if (dynamic_cast<Water*>(otherObject) != nullptr) {
		SphereVolume* volume = new SphereVolume(0.3f);
		boundingVolume = (CollisionVolume*)volume;
	}*/
}

void PlayerObject::OnCollisionEnd(GameObject* otherObject)
{
}

void PlayerObject::move(Vector3 axis)
{
	physicsObject->AddForce(axis * moveForce);
}
