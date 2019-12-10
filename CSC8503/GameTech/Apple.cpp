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
	followHeight = 3.0f;
	delay = 0.0f;
}

void Apple::followPlayer(float dt)
{
	if (taken) {
		transform.SetWorldPosition(Vector3(transform.GetWorldPosition().x, followHeight, transform.GetWorldPosition().z));
		Debug::DrawLine(currentGoalPos, Vector3(currentGoalPos.x, 7.0f, currentGoalPos.z), Vector4(1, 1, 1, 1)); // TODO: delete this
		float distanceFromGoal = (transform.GetWorldPosition() - currentGoalPos).Length();
		Vector3 axis = (currentGoalPos - transform.GetWorldPosition());
		axis.Normalise();
		if (distanceFromGoal > followHeight * 2.0f) {
			physicsObject->AddForce(axis * followSpeed);
		} else {
			if (navGrid->FindPath(transform.GetWorldPosition(), player->GetTransform().GetWorldPosition(), pathToPlayer)) {
				//getRightGoalPos(currentGoalPos);
				pathToPlayer.PopWaypoint(currentGoalPos);
				pathToPlayer.PopWaypoint(currentGoalPos);
				pathToPlayer.PopWaypoint(currentGoalPos);
				physicsObject->AddForce(axis * followSpeed);
			}
		}
	}
}

void Apple::getRightGoalPos(Vector3 goalPos)
{
	NavigationPath copy = pathToPlayer;
	Vector3 oldGoal = currentGoalPos;
	float distOldGoal = (oldGoal - goalPos).Length();
	float distNewGoal = (Vector3(0, 0, 0) - goalPos).Length();

	for (int i = 0; i < 3 && distOldGoal <= distNewGoal; i++) {
		pathToPlayer.PopWaypoint(currentGoalPos);
		distNewGoal = (currentGoalPos - goalPos).Length();
	}
	if (distOldGoal < distNewGoal) {
		pathToPlayer = copy;
		pathToPlayer.PopWaypoint(currentGoalPos);
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
