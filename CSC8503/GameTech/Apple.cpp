#include "Apple.h"

Apple::Apple(Vector3 position, MeshGeometry* mesh, ShaderBase* shader)
{
	SphereVolume* volume = new SphereVolume(0.5f);
	boundingVolume = (CollisionVolume*)volume;
	transform.SetWorldScale(Vector3(2, 2, 2));
	transform.SetWorldPosition(position);

	renderObject = new RenderObject(&transform, mesh, nullptr, shader);
	physicsObject = new PhysicsObject(&transform, boundingVolume);
	physicsObject->SetInverseMass(1.0f);
	physicsObject->InitSphereInertia();
}

void Apple::OnCollisionBegin(GameObject* otherObject)
{
	if (dynamic_cast<PlayerObject*>(otherObject) != nullptr) {
		transform.SetWorldScale(Vector3(4, 4, 4));
	}
}
