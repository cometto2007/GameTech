#include "Water.h"

Water::Water(Vector3 position, Vector3 waterSize, MeshGeometry* mesh, ShaderBase* shader)
{
	AABBVolume* volume = new AABBVolume(waterSize);
	boundingVolume = (CollisionVolume*)volume;
	transform.SetWorldScale(waterSize);
	transform.SetWorldPosition(position);

	renderObject = new RenderObject(&transform, mesh, nullptr, shader);
	physicsObject = new PhysicsObject(&transform, boundingVolume);
	physicsObject->SetInverseMass(0);
	physicsObject->InitCubeInertia();

	renderObject->SetColour(Vector4(0.09f, 0.76f, 0.92f, 1.0f));
}
