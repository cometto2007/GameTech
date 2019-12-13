#include "HumanEnemy.h"
#include "PatrolState.h"

#include <math.h> 

HumanEnemy::HumanEnemy(Vector3 position, MeshGeometry* mesh, ShaderBase* shader, CourseworkGame* game, bool isParkKeeper)
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
	pushdownMachBehav->addState(new PatrolState(this, game->getPlayer(), isParkKeeper));
	navGrid = loader.getHumanGrid(); // TODO: change in the new grid for humans
	speed = 30.0f;
	this->game = game;
	this->isParkKeeper = isParkKeeper;
}

HumanEnemy::~HumanEnemy()
{
	delete pushdownMachBehav;
}

void HumanEnemy::followPosition(Vector3 position)
{
	if (navGrid->isPossible(position)) {
		if (path.isEmpty()) {
			navGrid->FindPath(transform.GetWorldPosition(), position, path);
			path.PopWaypoint(currentGoalPos);
		}

		float angle = std::atan((transform.GetWorldPosition().x - position.x) / (transform.GetWorldPosition().y - position.y));
		angle = angle * (180.0 / 3.141592653589793238463);
		transform.SetLocalOrientation(Quaternion::AxisAngleToQuaterion(Vector3(0,1,0), -angle));

		float distanceFromGoal = (transform.GetWorldPosition() - currentGoalPos).Length();
		Vector3 axis = (currentGoalPos - transform.GetWorldPosition());
		axis.Normalise();
		if (distanceFromGoal > 20.0f * 2.0f) {
			physicsObject->AddForce(axis * speed);
		}
		else {
			if (navGrid->FindPath(transform.GetWorldPosition(), position, path)) {
				getRightGoalPos(position);
				physicsObject->AddForce(axis * speed);
			}
		}
	}
}

void HumanEnemy::shootBall(Vector3 position)
{
	Vector3 fwd = position - transform.GetWorldPosition();
	GameObject* bullet = game->AddSphereToWorld(transform.GetWorldPosition() + fwd * 0.02f, 0.2f, 0.5f);
	fwd.Normalise();
	bullet->GetPhysicsObject()->AddForce(fwd * 20000.0f);
}

void HumanEnemy::getRightGoalPos(Vector3 goalPos)
{
	NavigationPath copy = path;
	Vector3 oldGoal = currentGoalPos;
	float distOldGoal = (oldGoal - goalPos).Length();
	float distNewGoal = (Vector3(0, 0, 0) - goalPos).Length();
	
	for (int i = 0; i < 3 && distOldGoal <= distNewGoal; i++) {
		path.PopWaypoint(currentGoalPos);
		distNewGoal = (currentGoalPos - goalPos).Length();
	}
	if (distOldGoal < distNewGoal) {
		path = copy;
		path.PopWaypoint(currentGoalPos);
	}
}
