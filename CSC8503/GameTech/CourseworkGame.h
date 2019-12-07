#pragma once
#include "GameTechRenderer.h"
#include "PlayerObject.h"
#include "Apple.h"
#include "Water.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/NavigationGrid.h"

namespace NCL {
	namespace CSC8503 {
		class CourseworkGame {
		public:
			CourseworkGame();
			~CourseworkGame();

			void InitWorld();
			void InitCamera();

			virtual void UpdateGame(float dt);

		protected:
			void InitialiseAssets();

			void UpdateKeys();
			bool SelectObject();

			void movePlayer();
			void LockedCameraMovement();

			GameObject* AddFloorToWorld(const Vector3& position, Vector3 floorSize, OGLTexture* text, Vector4 col);
			GameObject* AddParkKeeperToWorld(const Vector3& position);

			NavigationGrid* navGrid;

			GameTechRenderer* renderer;
			PhysicsSystem* physics;
			GameWorld* world;

			bool useGravity;
			bool inSelectionMode;

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

			vector<Apple*> apples;

			//Coursework Additional functionality	
			GameObject* lockedObject = nullptr;
			Vector3 lockedOffset = Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}
		};
	}
}

