#include "NetworkedGame.h"
#include "NetworkPlayer.h"
#include "../CSC8503Common/GameServer.h"
#include "../CSC8503Common/GameClient.h"

#define COLLISION_MSG 30

NetworkedGame::NetworkedGame(bool isServer)
{
	NetworkBase::Initialise();
	if (isServer) StartAsServer();
	else StartAsClient(127, 0, 0, 1);
}

NetworkedGame::~NetworkedGame()
{
	NetworkBase::Destroy();
}

void NetworkedGame::StartAsServer()
{
	thisServer = new GameServer(NetworkBase::GetDefaultPort(), 2);
	thisServer->setGame(this);
	thisServer->RegisterPacketHandler(Received_State, this);
	
	std::cout << "Server Connected" << std::endl;
}

void NetworkedGame::StartAsClient(char a, char b, char c, char d)
{
	thisClient = new GameClient();
	thisClient->RegisterPacketHandler(Full_State, this);
	thisClient->RegisterPacketHandler(Delta_State, this);
	thisClient->RegisterPacketHandler(Player_Connected, this);
	bool canConnect = thisClient->Connect(a, b, c, d, NetworkBase::GetDefaultPort());
	if (canConnect) std::cout << "Client Connected" << std::endl;
	else std::cout << "Impossible connect client" << std::endl;
}

void NetworkedGame::UpdateAsServer(float dt)
{
	BroadcastSnapshot(true);
}

void NetworkedGame::UpdateAsClient(float dt)
{
	ClientPacket newPacket;
	bool buttonPressed = false;
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		newPacket.buttonstates[0] = 1;
		buttonPressed = true;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		newPacket.buttonstates[1] = 1;
		buttonPressed = true;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		newPacket.buttonstates[2] = 1;
		buttonPressed = true;
	} 
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		newPacket.buttonstates[3] = 1;
		buttonPressed = true;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::SPACE)) {
		newPacket.buttonstates[4] = 1;
		buttonPressed = true;
	} 
	if (buttonPressed) {
		newPacket.lastID = thisClient->getNetworkState();
		thisClient->SendPacket(newPacket);
	}
}

void NetworkedGame::BroadcastSnapshot(bool deltaFrame)
{
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;
	
	world->GetObjectIterators(first, last);
	
	for (auto i = first; i != last; ++i) {
		NetworkObject * o = (*i)->GetNetworkObject();
		if (!o) {
			continue;
		}
		int playerState = 0; // You 'll need to do this bit !
		GamePacket * newPacket = nullptr;
		if (o->WritePacket(&newPacket, deltaFrame, playerState)) {
			thisServer->SendGlobalPacket(*newPacket); // change ...
			delete newPacket;
		}
	}
}

void NetworkedGame::UpdateMinimumState()
{
}

void NetworkedGame::UpdateGame(float dt)
{
	CourseworkGame::UpdateGame(dt);
	if (thisServer != nullptr) {
		UpdateAsServer(dt);
		thisServer->UpdateServer();
	} 
	if (thisClient != nullptr) {
		UpdateAsClient(dt);
		thisClient->UpdateClient();
	} 
}

void NetworkedGame::SpawnPlayer()
{
}

void NetworkedGame::StartLevel()
{
	TutorialGame::TutorialGame();
}

void NetworkedGame::ReceivePacket(int type, GamePacket* payload, int source)
{
	if (type == Received_State) {
		ClientPacket* cp = (ClientPacket*)payload;
		PlayerObject* netPlayer = (PlayerObject*)serverPlayers.at(source);
		if (cp->buttonstates[0]) {
			movePlayerByKey(KeyboardKeys::LEFT, netPlayer);
		}
		if (cp->buttonstates[1]) {
			movePlayerByKey(KeyboardKeys::RIGHT, netPlayer);
		}
		if (cp->buttonstates[2]) {
			movePlayerByKey(KeyboardKeys::DOWN, netPlayer);
		}
		if (cp->buttonstates[3]) {
			movePlayerByKey(KeyboardKeys::UP, netPlayer);
		}
		if (cp->buttonstates[4]) {
			movePlayerByKey(KeyboardKeys::SPACE, netPlayer);
		}
		GamePacket* gp = new GamePacket();
		netPlayer->GetNetworkObject()->WritePacket(&gp, true, cp->lastID);
		thisServer->SendGlobalPacket(*gp);
	} else if (type == Delta_State) {
		DeltaPacket* cp = (DeltaPacket*)payload;

	} else if (type == Player_Connected) {
		GamePacket* p;
	}
	


	// if client raise networkState
}

void NetworkedGame::OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b)
{
}
