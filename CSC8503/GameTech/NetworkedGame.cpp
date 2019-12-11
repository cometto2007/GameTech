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
	player->GetNetworkObject()->setNetworkId(100);
	
	std::cout << "Server Connected" << std::endl;
}

void NetworkedGame::StartAsClient(char a, char b, char c, char d)
{
	thisClient = new GameClient();
	thisClient->RegisterPacketHandler(Full_State, this);
	thisClient->RegisterPacketHandler(Delta_State, this);
	player->GetNetworkObject()->setNetworkId(101);

	bool canConnect = thisClient->Connect(a, b, c, d, NetworkBase::GetDefaultPort());
	if (canConnect) std::cout << "Client Connected" << std::endl;
	else std::cout << "Impossible connect client" << std::endl;
}

void NetworkedGame::UpdateAsServer(float dt)
{
	// TODO: add logic for delta and full packet
	BroadcastSnapshot(false);
}

void NetworkedGame::UpdateAsClient(float dt)
{
	if (!blockMovement) {
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
		Vector2 relativePos = Window::GetMouse()->GetRelativePosition();
		if (relativePos.x != 0) {
			newPacket.buttonstates[5] = 1;
			newPacket.xVar = relativePos.x;
			buttonPressed = true;
		}
		if (buttonPressed) {
			newPacket.lastID = thisClient->getNetworkState();
			thisClient->SendPacket(newPacket);
		}
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
		GamePacket * newPacket = nullptr;
		if (o->WritePacket(&newPacket, deltaFrame, playerState)) {
			thisServer->SendGlobalPacket(*newPacket);
			delete newPacket;
		}
	}
	GamePacket* newPacket = nullptr;
	if (player->GetNetworkObject()->WritePacket(&newPacket, deltaFrame, playerState)) {
		thisServer->SendGlobalPacket(*newPacket);
		delete newPacket;
	}
}

void NetworkedGame::UpdateMinimumState()
{
}

void NCL::CSC8503::NetworkedGame::changePlayerRotationFromVar(PlayerObject* obj, float var)
{
	Quaternion newOr = Quaternion::EulerAnglesToQuaternion(0, -var * 2.5f, 0) * obj->GetTransform().GetLocalOrientation();
	obj->GetTransform().SetLocalOrientation(newOr);
}

void NetworkedGame::UpdateGame(float dt)
{
	CourseworkGame::UpdateGame(dt);
	if (thisServer != nullptr) {
		Window::GetWindow()->SetTitle("Server");
		UpdateAsServer(dt);
		thisServer->UpdateServer();
	} 
	if (thisClient != nullptr) {
		UpdateAsClient(dt);
		Window::GetWindow()->SetTitle("Client");
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
	PlayerObject* netPlayer;
	if (type == Received_State) {
		netPlayer = getPlayerBySource(101);
		ClientPacket* cp = (ClientPacket*)payload;
		if (cp->buttonstates[0] == 1) {
			movePlayerByKey(KeyboardKeys::LEFT, netPlayer);
		}
		if (cp->buttonstates[1] == 1) {
			movePlayerByKey(KeyboardKeys::RIGHT, netPlayer);
		}
		if (cp->buttonstates[2] == 1) {
			movePlayerByKey(KeyboardKeys::DOWN, netPlayer);
		}
		if (cp->buttonstates[3] == 1) {
			movePlayerByKey(KeyboardKeys::UP, netPlayer);
		}
		if (cp->buttonstates[4] == 1) {
			movePlayerByKey(KeyboardKeys::SPACE, netPlayer);
		}
		if (cp->buttonstates[5] == 1) {
			changePlayerRotationFromVar(netPlayer, cp->xVar);
		}
		GamePacket* newPacket = nullptr;
		if (netPlayer->GetNetworkObject()->WritePacket(&newPacket, false, 101)) {
			thisServer->SendGlobalPacket(*newPacket);
			delete newPacket;
		}
	} else if (type == Full_State) {
		FullPacket* fp = (FullPacket*)payload;
		if (fp->objectID != 101) { // if 0 is the player itself
			netPlayer = getPlayerBySource(100);
			thisClient->setNetworkState(fp->fullState.stateID);
			if (netPlayer != nullptr) {
				netPlayer->GetNetworkObject()->ReadPacket(*fp);
			} else {
				PlayerObject* newNetPlayer = new PlayerObject(fp->fullState.position, gooseMesh, basicShader, true, true);
				newNetPlayer->GetNetworkObject()->setNetworkId(100);
				serverPlayers.insert(std::make_pair(100, newNetPlayer));
				world->AddGameObject(newNetPlayer);
			}
		} else {
			player->GetNetworkObject()->ReadPacket(*fp);
		}
	}
}

PlayerObject* NCL::CSC8503::NetworkedGame::getPlayerBySource(int source)
{
	PlayerObject* netPlayer;
	try {
		netPlayer = (PlayerObject*)serverPlayers.at(source);
	} catch (std::out_of_range& const e) {
		netPlayer = nullptr;
	}
	return netPlayer;
}

void NetworkedGame::OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b)
{
}
