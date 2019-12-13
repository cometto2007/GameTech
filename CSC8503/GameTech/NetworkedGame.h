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
			NetworkedGame(bool isServer);
			~NetworkedGame();

			void StartAsServer();
			void StartAsClient(char a, char b, char c, char d);

			void UpdateGame(float dt) override;

			void SpawnPlayer();

			void StartLevel();

			void ReceivePacket(int type, GamePacket* payload, int source) override;

			vector<int> getLeaderboard() { return leaderboards; }
			bool getIsFinish() { return gameIsFinish; }

			PlayerObject* getPlayerBySource(int source);

			void setGameIsFinish(bool b) { gameIsFinish = b; };

			void addServerPlayer(int id) { 
				PlayerObject* netPl = new PlayerObject(Vector3(55, 2, 150), gooseMesh, basicShader, true, true);
				netPl->GetNetworkObject()->setNetworkId(101);
				serverPlayers.insert(std::make_pair(101, netPl));
				world->AddGameObject(netPl);
			}

			void OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b);

		protected:
			void UpdateAsServer(float dt);
			void UpdateAsClient(float dt);
			vector<string> splitStringIntoVector(string s);

			void BroadcastSnapshot(bool deltaFrame);
			void UpdateMinimumState();

			void changePlayerRotationFromVar(PlayerObject* obj, float var);

			std::map<int, int> stateIDs;

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;
			int g2Points;
			bool gameIsFinish = false;

			vector<int> leaderboards;

			std::vector<NetworkObject*> networkObjects;
			std::map<int, GameObject*> serverPlayers;
			int playerState = 0;
			int currentServerPacket = 0;
			GameObject* localPlayer;
		};
	}
}

