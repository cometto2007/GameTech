#include "CourseworkGame.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"
#include "Utility.h"
#include "HumanEnemy.h"
#include "../CSC8503Common/PositionConstraint.h"
#include "Menu.h"

using namespace NCL;
using namespace CSC8503;

CourseworkGame::CourseworkGame() {
	world = new GameWorld();
	renderer = new GameTechRenderer(*world);
	physics = new PhysicsSystem(*world);
	navGrid = new NavigationGrid("TestGrid1.txt");
	blockMovement = false;

	forceMagnitude = 10.0f;
	useGravity = false;
	inSelectionMode = false;

	Debug::SetRenderer(renderer);

	InitialiseAssets();
}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes,
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void CourseworkGame::InitialiseAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh", &cubeMesh);
	loadFunc("sphere.msh", &sphereMesh);
	loadFunc("goose.msh", &gooseMesh);
	loadFunc("CharacterA.msh", &keeperMesh);
	loadFunc("CharacterM.msh", &charA);
	loadFunc("CharacterF.msh", &charB);
	loadFunc("Apple.msh", &appleMesh);

	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	InitCamera();
	InitWorld();
}

CourseworkGame::~CourseworkGame() {
	delete cubeMesh;
	delete sphereMesh;
	delete gooseMesh;
	delete basicTex;
	delete basicShader;

	delete physics;
	delete renderer;
	delete world;
}

void CourseworkGame::UpdateGame(float dt) {
	if (!inSelectionMode) {
		world->GetMainCamera()->UpdateCamera(dt);
	}
	if (lockedObject != nullptr) {
		LockedCameraMovement();
	}

	UpdateKeys();

	/*if (useGravity) {
		Debug::Print("(G)ravity on", Vector2(10, 40));
	}
	else {
		Debug::Print("(G)ravity off", Vector2(10, 40));
	}*/

	double secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
	remainingTime = (int)(gameSecDuration - secondsPassed);
	Vector2 pos = Window::GetWindow()->GetScreenSize();
	Debug::Print("Time " + std::to_string(remainingTime), Vector2(pos.x / 2 - 150, pos.y - 40));
	Debug::Print("Points " + std::to_string(points), Vector2(pos.x / 2 - 150, pos.y - 60));
	SelectObject();

	world->UpdateWorld(dt);
	renderer->Update(dt);
	physics->Update(dt);

	Debug::FlushRenderables();

	for (Apple* a : apples) {
		a->followPlayer(dt);
	}
	e->getBehaviour()->Update();
	checkLetApples();
	renderer->Render();
}

void CourseworkGame::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1)) {
		InitWorld(); //We can reset the simulation at any time with F1
		selectionObject = nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
		useGravity = !useGravity; //Toggle gravity!
		physics->UseGravity(useGravity);
	}

	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8)) {
		world->ShuffleObjects(false);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::B)) {
		blockMovement = true;
	}

	if (!blockMovement) {
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) movePlayerByKey(KeyboardKeys::LEFT, player);
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) movePlayerByKey(KeyboardKeys::RIGHT, player);
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) movePlayerByKey(KeyboardKeys::DOWN, player);
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) movePlayerByKey(KeyboardKeys::UP, player);
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::SPACE)) movePlayerByKey(KeyboardKeys::SPACE, player);
	}

	float yaw = Window::GetMouse()->GetRelativePosition().x;
	Quaternion newOr = Quaternion::EulerAnglesToQuaternion(0, -yaw * 2.5f, 0) * player->GetTransform().GetLocalOrientation();
	player->GetTransform().SetLocalOrientation(newOr);
}

void  CourseworkGame::LockedCameraMovement() {
	if (lockedObject != nullptr) {
		Vector3 objPos = lockedObject->GetTransform().GetWorldPosition();
		Vector3 camPos = objPos + lockedOffset;

		Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0, 1, 0));

		Matrix4 modelMat = temp.Inverse();

		Quaternion q(modelMat);
		Vector3 angles = q.ToEuler(); //nearly there now!

		world->GetMainCamera()->SetPosition(camPos);
		world->GetMainCamera()->SetPitch(angles.x + 5);
		world->GetMainCamera()->SetYaw(angles.y);
	}
}

/*

Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around.

*/
bool CourseworkGame::SelectObject() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		if (selectionObject != nullptr) {
			Vector3 pos = selectionObject->GetTransform().GetWorldPosition();
			Vector3 or = selectionObject->GetTransform().GetWorldOrientation().ToEuler();
			renderer->DrawString("position x: " + std::to_string(pos.x), Vector2(10, 40));
			renderer->DrawString("position y: " + std::to_string(pos.y), Vector2(10, 20));
			renderer->DrawString("position z: " + std::to_string(pos.z), Vector2(10, 0));

			renderer->DrawString("orientation x: " + std::to_string(or.x), Vector2(10, 100));
			renderer->DrawString("orientation y: " + std::to_string(or.y), Vector2(10, 80));
			renderer->DrawString("orientation z: " + std::to_string(or.z), Vector2(10, 60));
		}

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				selectionObject = (GameObject*)closestCollision.node;
				return true;
			} else {
				selectionObject = nullptr;
				return false;
			}
		}
	}
	return false;
}

void CourseworkGame::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.5f);
	world->GetMainCamera()->SetFarPlane(1000.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(315.0f);
	world->GetMainCamera()->SetPosition(Vector3(-60, 40, 60));
	lockedObject = nullptr;
}

void CourseworkGame::InitWorld() {
	world->ClearAndErase();
	apples.clear();
	physics->Clear();

	startTime = clock(); 
	gameSecDuration = 180;
	remainingTime = gameSecDuration;

	player = new PlayerObject(Vector3(50, 2, 150), gooseMesh, basicShader, true);
	world->AddGameObject(player);
	LockCameraToObject(player);

	islandPosition = Vector3(50, -1, 150);

	AddFloorToWorld(Vector3(50, -1, 50), Vector3(50, 1, 50), nullptr, Vector4(0.16f, 0.71f, 0.0f, 1.0f));
	AddFloorToWorld(islandPosition, Vector3(12.5f, 1, 12.5f), nullptr, Vector4(0.16f, 0.71f, 0.0f, 1.0f));
	AddFloorToWorld(Vector3(150, -1, 150), Vector3(50, 1, 50), nullptr, Vector4(0.16f, 0.71f, 0.0f, 1.0f));
	world->AddGameObject(new Water(Vector3(50, -2, 150), Vector3(50, 1, 50), cubeMesh, basicShader));

	AddFloorToWorld(Vector3(150, -1, 90), Vector3(50, 1, 10), nullptr, Vector4(0.16f, 0.71f, 0.0f, 1.0f));
	AddFloorToWorld(Vector3(150, -1, 35), Vector3(50, 1, 35), nullptr, Vector4(0.16f, 0.71f, 0.0f, 1.0f));
	AddFloorToWorld(Vector3(167.5f, -1, 75), Vector3(32.5f, 1, 5), nullptr, Vector4(0.16f, 0.71f, 0.0f, 1.0f));

	for (size_t i = 0; i < 20; i++) {
		Apple* a = new Apple(Vector3(RandomFloat(0, 100), 3, RandomFloat(0, 100)), appleMesh, basicShader);
		a->GetRenderObject()->SetColour(Vector4(0.85f, 0.22f, 0.14f, 1.0f));
		apples.push_back(a);
		world->AddGameObject(a);
	}
	/*Apple* a = new Apple(Vector3(55, 3, 150), appleMesh, basicShader);
	a->GetRenderObject()->SetColour(Vector4(0.85f, 0.22f, 0.14f, 1.0f));
	apples.push_back(a);
	world->AddGameObject(a);*/

	e = new HumanEnemy(Vector3(150, 3, 50), charA, basicShader, this);
	world->AddGameObject(e);

	AddBridgeConstraint();
}

//From here on it's functions to add in objects to the world!

/*

A single function to add a large immoveable cube to the bottom of our world

*/
GameObject* CourseworkGame::AddFloorToWorld(const Vector3& position, Vector3 floorSize, OGLTexture* text, Vector4 col) {
	GameObject* floor = new GameObject();

	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform().SetWorldScale(floorSize);
	floor->GetTransform().SetWorldPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, text, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	if (col != Vector4()) {
		floor->GetRenderObject()->SetColour(col);
	}

	world->AddGameObject(floor);
	return floor;
}

GameObject* CourseworkGame::AddParkKeeperToWorld(const Vector3& position)
{
	float meshSize = 4.0f;
	float inverseMass = 0.5f;

	GameObject* keeper = new GameObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.3, 0.9f, 0.3) * meshSize);
	keeper->SetBoundingVolume((CollisionVolume*)volume);

	keeper->GetTransform().SetWorldScale(Vector3(meshSize, meshSize, meshSize));
	keeper->GetTransform().SetWorldPosition(position);

	keeper->SetRenderObject(new RenderObject(&keeper->GetTransform(), keeperMesh, nullptr, basicShader));
	keeper->SetPhysicsObject(new PhysicsObject(&keeper->GetTransform(), keeper->GetBoundingVolume()));

	keeper->GetPhysicsObject()->SetInverseMass(inverseMass);
	keeper->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(keeper);

	return keeper;
}

void NCL::CSC8503::CourseworkGame::movePlayerByKey(KeyboardKeys k, PlayerObject* obj)
{
	Matrix4 view = world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();
	Vector3 rightAxis = Vector3(camWorld.GetColumn(0));
	Vector3 fwdAxis = obj->GetTransform().GetLocalMatrix().GetColumn(2);

	if (k == KeyboardKeys::LEFT) obj->move(-rightAxis);
	if (k == KeyboardKeys::RIGHT) obj->move(rightAxis);
	if (k == KeyboardKeys::DOWN) obj->move(-fwdAxis);
	if (k == KeyboardKeys::UP) obj->move(fwdAxis);
	if (k == KeyboardKeys::SPACE) obj->move(Vector3(0, 1, 0));
}

GameObject* CourseworkGame::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject();

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);
	sphere->GetTransform().SetWorldScale(sphereSize);
	sphere->GetTransform().SetWorldPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}

void CourseworkGame::AddBridgeConstraint() {
	Vector3 cubeSize = Vector3(1.5f, 0.1f, 4);

	float	invCubeMass = 1;
	int		numLinks = 4;
	float	maxDistance = 5.5f;
	float	cubeDistance = 5;

	Vector3 startPos = Vector3(100, 0, 75);
	Vector4 col(0.72f, 0.44f, 0.05f, 1.0f);

	GameObject* start = AddCubeToWorld(startPos + Vector3(2, -0.5f, 0), cubeSize, 0, col);
	GameObject* end = AddCubeToWorld(startPos + Vector3((numLinks + 2) * cubeDistance, -0.5f, 0), cubeSize, 0, col);
	GameObject* previous = start;

	for (int i = 0; i < numLinks; ++i) {
		GameObject* block = AddCubeToWorld(startPos + Vector3((i + 1) * cubeDistance, 0, 0), cubeSize, invCubeMass, col);
		PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);
		world->AddConstraint(constraint);
		previous = block;
	}

	PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
	world->AddConstraint(constraint);
}

GameObject* CourseworkGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, Vector4 col)
{
	GameObject* cube = new GameObject();
	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetWorldPosition(position);
	cube->GetTransform().SetWorldScale(dimensions);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, nullptr, basicShader));
	cube->GetRenderObject()->SetColour(col);
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

void NCL::CSC8503::CourseworkGame::checkLetApples()
{
	float distance = (player->GetTransform().GetWorldPosition() - islandPosition).Length();
	if (distance < 10) {
		for (size_t i = 0; i < apples.size(); i++) {
			if (apples[i]->getIsTaken()) {
				world->RemoveGameObject(apples[i]);
				apples.erase(apples.begin() + i);
				points++;
			}
		}
	}
}
