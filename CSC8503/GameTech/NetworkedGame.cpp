#include "NetworkedGame.h"
#include "NetworkPlayer.h"
#include "../CSC8503Common/GameServer.h"
#include "../CSC8503Common/GameClient.h"

#define COLLISION_MSG 30

NetworkedGame::NetworkedGame()
{
	NetworkBase::Initialise();
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
	bool canConnect = thisClient->Connect(a, b, c, d, NetworkBase::GetDefaultPort());
	if (canConnect) std::cout << "Client Connected" << std::endl;
	else std::cout << "Impossible connect client" << std::endl;
}

void NetworkedGame::UpdateAsServer(float dt)
{
	
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
	TutorialGame::UpdateGame(dt);
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
	if (thisClient != nullptr) {
		ghostGoose->SetNetworkObject(new NetworkObject(*ghostGoose, 2));
	}
}

void NetworkedGame::ReceivePacket(int type, GamePacket* payload, int source)
{
	if (type == Received_State) {
		ClientPacket* cp = (ClientPacket*)payload;
		if (cp->buttonstates[0]) {
			moveObject(serverPlayers.at(source), KeyboardKeys::LEFT);
		}
		if (cp->buttonstates[1]) {
			moveObject(serverPlayers.at(source), KeyboardKeys::RIGHT);
		}
		if (cp->buttonstates[2]) {
			moveObject(serverPlayers.at(source), KeyboardKeys::DOWN);
		}
		if (cp->buttonstates[3]) {
			moveObject(serverPlayers.at(source), KeyboardKeys::UP);
		}
		if (cp->buttonstates[4]) {
			moveObject(serverPlayers.at(source), KeyboardKeys::SPACE);
		}
		GamePacket* gp;
		serverPlayers.at(source)->GetNetworkObject()->WritePacket(&gp, true, cp->lastID++);
		thisServer->SendGlobalPacket(*gp);
	}
	else if (type == Delta_State) {
		DeltaPacket* cp = (DeltaPacket*)payload;

	}
	


	// if client raise networkState
}

void NetworkedGame::OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b)
{
}
