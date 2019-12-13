#include "bridgeLog.h"

bridgeLog::bridgeLog(Vector3 position, Vector3 logSize, MeshGeometry* mesh, ShaderBase* shader, GameWorld* world)
{
	AABBVolume* volume = new AABBVolume(logSize);
	boundingVolume = (CollisionVolume*)volume;
	transform.SetWorldScale(logSize);
	transform.SetWorldPosition(position);

	renderObject = new RenderObject(&transform, mesh, nullptr, shader);
	physicsObject = new PhysicsObject(&transform, boundingVolume);
	physicsObject->SetInverseMass(0);
	physicsObject->InitCubeInertia();

	renderObject->SetColour(Vector4(0.72f, 0.44f, 0.05f, 1.0f));
	this->world = world;
	world->AddGameObject(this);
}

void bridgeLog::OnCollisionEnd(GameObject* otherObject)
{
	if (dynamic_cast<PlayerObject*>(otherObject) != nullptr) {
		if (world->getConstraintSize() == 5) {
			//world->RemoveConstraint(world->getConstraint(0));
		}
	}
}
