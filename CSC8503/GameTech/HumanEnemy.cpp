#include "HumanEnemy.h"
#include "PatrolState.h"

HumanEnemy::HumanEnemy(Vector3 position, MeshGeometry* mesh, ShaderBase* shader)
{
	float meshSize = 4.0f;
	float inverseMass = 0.5f;

	AABBVolume* volume = new AABBVolume(Vector3(0.3, 0.9f, 0.3) * meshSize);
	boundingVolume = (CollisionVolume*)volume;

	transform.SetWorldScale(Vector3(meshSize, meshSize, meshSize));
	transform.SetWorldPosition(position);

	renderObject = new RenderObject(&transform, mesh, nullptr, shader);
	physicsObject =  new PhysicsObject(&transform, boundingVolume);

	physicsObject->SetInverseMass(inverseMass);
	physicsObject->InitCubeInertia();

	pushdownMachBehav = new PushdownMachine();
	pushdownMachBehav->addState(new PatrolState(this));
	navGrid = loader.getFloatingGrid(); // TODO: change in the new grid for humans
	speed = 7.0f;
}

HumanEnemy::~HumanEnemy()
{
	delete pushdownMachBehav;
}

void HumanEnemy::followPosition(Vector3 position)
{
	Debug::DrawLine(position, Vector3(position.x, 7.0f, position.z), Vector4(1, 0, 0, 1));
	if (path.isEmpty()) {
		navGrid->FindPath(transform.GetWorldPosition(), position, path);
		path.PopWaypoint(currentGoalPos);
	}
	// TODO: debug porpose
	Debug::DrawLine(currentGoalPos, Vector3(currentGoalPos.x, 7.0f, currentGoalPos.z), Vector4(1, 1, 1, 1));
	
	transform.SetWorldPosition(Vector3(transform.GetWorldPosition().x, 20, transform.GetWorldPosition().z));
	float distanceFromGoal = (transform.GetWorldPosition() - currentGoalPos).Length();
	std::cout << distanceFromGoal << std::endl;
	if (distanceFromGoal > 20.0f * 2.0f) {
		physicsObject->AddForce((currentGoalPos - transform.GetWorldPosition()) * speed);
	} else {
		if (navGrid->FindPath(transform.GetWorldPosition(), position, path)) {
			path.PopWaypoint(currentGoalPos);
			path.PopWaypoint(currentGoalPos);
			path.PopWaypoint(currentGoalPos);
			physicsObject->AddForce((currentGoalPos - transform.GetWorldPosition()) * speed);
		}
	}
}


