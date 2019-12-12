#pragma once
#include "GameTechRenderer.h"
#include "PlayerObject.h"
#include "Apple.h"
#include "Water.h"
//#include "HumanEnemy.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/NavigationGrid.h"

class HumanEnemy;
namespace NCL {
	namespace CSC8503 {
		class CourseworkGame {
		public:
			CourseworkGame();
			~CourseworkGame();

			void InitWorld();
			void InitCamera();

			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass);
			void AddObjectToWorld(GameObject* obj) { world->AddGameObject(obj); };
			PlayerObject* getPlayer() { return player; };
			int getRemainingTime() { return remainingTime; };
			int getPoints() { return points; }

			virtual void UpdateGame(float dt);

		protected:
			void InitialiseAssets();

			void UpdateKeys();
			bool SelectObject();
			void LockedCameraMovement();

			GameObject* AddFloorToWorld(const Vector3& position, Vector3 floorSize, OGLTexture* text, Vector4 col);
			GameObject* AddParkKeeperToWorld(const Vector3& position);

			void movePlayerByKey(KeyboardKeys k, PlayerObject* obj);

			NavigationGrid* navGrid;

			GameTechRenderer* renderer;
			PhysicsSystem* physics;
			GameWorld* world;

			bool useGravity;
			bool inSelectionMode;
			bool blockMovement;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;
			PlayerObject* player = nullptr;

			OGLMesh* cubeMesh = nullptr;
			OGLMesh* sphereMesh = nullptr;
			OGLTexture* basicTex = nullptr;
			OGLShader* basicShader = nullptr;

			//Coursework Meshes
			OGLMesh* gooseMesh = nullptr;
			OGLMesh* keeperMesh = nullptr;
			OGLMesh* appleMesh = nullptr;
			OGLMesh* charA = nullptr;
			OGLMesh* charB = nullptr;

			int points = 0;

			vector<Apple*> apples;
			HumanEnemy* e;
			Vector3 islandPosition;

			// Timer
			clock_t startTime;
			double gameSecDuration;
			int remainingTime;

			//Coursework Additional functionality	
			GameObject* lockedObject = nullptr;
			Vector3 lockedOffset = Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}
			void AddBridgeConstraint();
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, Vector4 col);
			void checkLetApples();
		};
	}
}

