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

	testPos = Vector3(50, 2, 150);
	navGrid = loader.getFloatingGrid();
	player = nullptr;
	taken = false;
	followSpeed = 15.0f;
	followHeight = 3.0f;
	delay = 0.0f;
}

void Apple::followPlayer(float dt)
{
	if (taken) {
		transform.SetWorldPosition(Vector3(transform.GetWorldPosition().x, followHeight, transform.GetWorldPosition().z));
		Debug::DrawLine(currentGoalPos, Vector3(currentGoalPos.x, 7.0f, currentGoalPos.z), Vector4(1, 1, 1, 1)); // TODO: delete this
		float distanceFromGoal = (transform.GetWorldPosition() - currentGoalPos).Length();
		std::cout << distanceFromGoal << std::endl;
		if (distanceFromGoal > followHeight * 2.0f) {
			physicsObject->AddForce((currentGoalPos - transform.GetWorldPosition()) * followSpeed);
		} else {
			if (navGrid->FindPath(transform.GetWorldPosition(), player->GetTransform().GetWorldPosition(), pathToPlayer)) {
				pathToPlayer.PopWaypoint(currentGoalPos);
				pathToPlayer.PopWaypoint(currentGoalPos);
				pathToPlayer.PopWaypoint(currentGoalPos);
				physicsObject->AddForce((currentGoalPos - transform.GetWorldPosition()) * followSpeed);
			}
		}
	}
}

void Apple::OnCollisionBegin(GameObject* otherObject)
{
	if (dynamic_cast<PlayerObject*>(otherObject) != nullptr) {
		transform.SetWorldScale(Vector3(1, 1, 1));
		if (player == nullptr) {
			player = (PlayerObject*)otherObject;
		}
	}
}

void Apple::OnCollisionEnd(GameObject* otherObject)
{
	if (dynamic_cast<PlayerObject*>(otherObject) != nullptr) {
		navGrid->FindPath(transform.GetWorldPosition(), player->GetTransform().GetWorldPosition(), pathToPlayer);
		pathToPlayer.PopWaypoint(currentGoalPos);
		taken = true;
	}
}
