#pragma once
#include <thread>
#include <atomic>

#include "NetworkBase.h"

namespace NCL {
	namespace CSC8503 {
		class GameWorld;
		class NetworkedGame;
		class GameServer : public NetworkBase {
		public:
			GameServer(int onPort, int maxClients);
			~GameServer();

			bool Initialise();
			void Shutdown();

			void SetGameWorld(GameWorld& g);
			void setGame(NetworkedGame* netGame) { game = netGame; };

			//void ThreadedUpdate();

			bool SendGlobalPacket(int msgID);
			bool SendGlobalPacket(GamePacket& packet);

			virtual void UpdateServer();

		protected:
			int			port;
			int			clientMax;
			int			clientCount;
			GameWorld*	gameWorld;
			NetworkedGame* game;

			//std::atomic<bool> threadAlive;

			

			//std::thread updateThread;

			int incomingDataRate;
			int outgoingDataRate;
		};
	}
}
