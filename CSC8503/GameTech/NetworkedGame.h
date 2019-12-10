#pragma once
#include "TutorialGame.h"
#include "CourseworkGame.h"

namespace NCL {
	namespace CSC8503 {
		class GameServer;
		class GameClient;
		class NetworkPlayer;

		class NetworkedGame : public CourseworkGame, public PacketReceiver {
		public:
			NetworkedGame();
			~NetworkedGame();

			void StartAsServer();
			void StartAsClient(char a, char b, char c, char d);

			void UpdateGame(float dt) override;

			void SpawnPlayer();

			void StartLevel();

			void ReceivePacket(int type, GamePacket* payload, int source) override;

			void addServerPlayer(int id) { 
				serverPlayers.insert(std::make_pair(id, nullptr)); // TODO: FIXXXXXXX
			}

			void OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b);

		protected:
			void UpdateAsServer(float dt);
			void UpdateAsClient(float dt);

			void BroadcastSnapshot(bool deltaFrame);
			void UpdateMinimumState();

			std::map<int, int> stateIDs;

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;

			std::vector<NetworkObject*> networkObjects;
			std::map<int, GameObject*> serverPlayers;
			GameObject* localPlayer;
		};
	}
}

