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
	//navGrid->FindPath(transform.GetWorldPosition(), player->GetTransform().GetWorldPosition(), pathToPlayer);
	//pathToPlayer.PopWaypoint(currentGoalPos);
	taken = false;
	delay = 0.0f;
}

void Apple::followPlayer(float dt)
{
	if (taken) {
		Debug::DrawLine(currentGoalPos, Vector3(currentGoalPos.x, 20, currentGoalPos.z), Vector4(1, 1, 1, 1));
		float distanceFromGoal = (transform.GetWorldPosition() - currentGoalPos).Length();
		std::cout << distanceFromGoal << std::endl;
		if (distanceFromGoal > 2.0f) {
			physicsObject->AddForce((currentGoalPos - transform.GetWorldPosition()) * 5.0f);
		}
		else {
			navGrid->FindPath(transform.GetWorldPosition(), player->GetTransform().GetWorldPosition(), pathToPlayer);

			pathToPlayer.PopWaypoint(currentGoalPos);
			pathToPlayer.PopWaypoint(currentGoalPos);
			pathToPlayer.PopWaypoint(currentGoalPos);
			physicsObject->SetAngularVelocity(Vector3(0, 0, 0));
			physicsObject->SetLinearVelocity(Vector3(0, 0, 0));
			physicsObject->AddForce((currentGoalPos - transform.GetWorldPosition()) * 15.0f);
		}
	}
	/*if (taken) {
		pathToPlayer.DrawDebugLine();
		Debug::DrawLine(currentGoalPos, Vector3(currentGoalPos.x, 20, currentGoalPos.z), Vector4(1, 1, 1, 1));

		// recalc after 2 sec
		delay += dt;
		if (delay > 3) {
			delay = 0.0f;
			navGrid->FindPath(transform.GetWorldPosition(), player->GetTransform().GetWorldPosition(), pathToPlayer);
			pathToPlayer.PopWaypoint(currentGoalPos);
		}
			
		float distanceFromGoal = (transform.GetWorldPosition() - currentGoalPos).Length();
		std::cout << distanceFromGoal << std::endl;
		if (distanceFromGoal > 0.5f) {
			physicsObject->AddForce((currentGoalPos - transform.GetWorldPosition()) * 1.0f);
		} else {
			navGrid->FindPath(transform.GetWorldPosition(), player->GetTransform().GetWorldPosition(), pathToPlayer);
			pathToPlayer.PopWaypoint(currentGoalPos);
		}
	}*/
}

void Apple::OnCollisionBegin(GameObject* otherObject)
{
	if (dynamic_cast<PlayerObject*>(otherObject) != nullptr) {
		transform.SetWorldScale(Vector3(1, 1, 1));
		player = (PlayerObject*)otherObject;
		taken = true;
	}
}
