#include "NetworkedGame.h"
#include "NetworkPlayer.h"
#include "../CSC8503Common/GameServer.h"
#include "../CSC8503Common/GameClient.h"
#include <algorithm>
#include <regex>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>


#define COLLISION_MSG 30

NetworkedGame::NetworkedGame(bool isServer)
{
	NetworkBase::Initialise();
	if (isServer) StartAsServer();
	else StartAsClient(127, 0, 0, 1);
}

NetworkedGame::~NetworkedGame()
{
	delete this->thisServer;
	this->thisClient->Destroy();
}

void NetworkedGame::StartAsServer()
{
	thisServer = new GameServer(NetworkBase::GetDefaultPort(), 2);
	thisServer->setGame(this);
	thisServer->RegisterPacketHandler(Received_State, this);
	thisServer->RegisterPacketHandler(String_Message, this);
	thisServer->RegisterPacketHandler(Player_Disconnected, this);
	player->GetNetworkObject()->setNetworkId(100);
	
	std::cout << "Server Connected" << std::endl;
}

void NetworkedGame::StartAsClient(char a, char b, char c, char d)
{
	thisClient = new GameClient();
	thisClient->RegisterPacketHandler(Full_State, this);
	thisClient->RegisterPacketHandler(Delta_State, this);
	thisClient->RegisterPacketHandler(String_Message, this);
	player->GetNetworkObject()->setNetworkId(101);

	bool canConnect = thisClient->Connect(a, b, c, d, NetworkBase::GetDefaultPort());
	if (canConnect) std::cout << "Client Connected" << std::endl;
	else std::cout << "Impossible connect client" << std::endl;
}

void NetworkedGame::UpdateAsServer(float dt)
{
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

vector<string> NCL::CSC8503::NetworkedGame::splitStringIntoVector(string s)
{
	std::vector<std::string> result;
	std::istringstream iss(s);
	for (std::string s; iss >> s; )
		result.push_back(s);
	return result;
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
	float screenY = Window::GetWindow()->GetScreenSize().y;
	float screenX = Window::GetWindow()->GetScreenSize().x / 2;
		
	CourseworkGame::UpdateGame(dt);
	if (thisServer != nullptr) {
		Window::GetWindow()->SetTitle("Server");
		UpdateAsServer(dt);
		renderer->DrawString("g1 " + std::to_string(points), Vector2(20, screenY - 20), Vector4(1, 0, 0, 1));
		renderer->DrawString("g2 " + std::to_string(g2Points), Vector2(20, screenY - 40), Vector4(1, 0, 0, 1));
		thisServer->UpdateServer();
	}
	if (thisClient != nullptr) {
		UpdateAsClient(dt);
		Window::GetWindow()->SetTitle("Client");
		renderer->DrawString("g1 " + std::to_string(points), Vector2(20, screenY - 20), Vector4(1, 0, 0, 1));
		renderer->DrawString("g2 " + std::to_string(g2Points), Vector2(20, screenY - 40), Vector4(1, 0, 0, 1));
		thisClient->UpdateClient();
	}
	if (!gameIsFinish) {
		if (thisServer != nullptr) {
			this->thisServer->SendGlobalPacket(StringPacket(std::to_string(points)));
		}
		else {
			this->thisClient->SendPacket(StringPacket(std::to_string(points)));
		}
		if (apples.size() <= 0) {
			gameIsFinish = true;
			if (thisServer != nullptr) {
				thisServer->SendGlobalPacket(StringPacket(" winner"));
			} else {
				thisClient->SendPacket(StringPacket(" winner " + std::to_string(points)));
			}
		}
	} else {
		sort(leaderboards.begin(), leaderboards.end(), std::greater<>());
		renderer->DrawString("Respawn in " + std::to_string(1), Vector2(screenX - 200, screenY - 20), Vector4(1, 1, 0, 1));
		renderer->DrawString("Leaderboards", Vector2(screenX - 200, screenY / 2 + 20), Vector4(1, 1, 0, 1));
		for (size_t i = 0; i < leaderboards.size(); i++) {
			renderer->DrawString(std::to_string(leaderboards[i]), Vector2(screenX, screenY/2 - i * 25), Vector4(1, 1, 0, 1));
		}
	}
}

void NetworkedGame::SpawnPlayer()
{
}

void NetworkedGame::StartLevel()
{
}

void NetworkedGame::ReceivePacket(int type, GamePacket* payload, int source)
{
	PlayerObject* netPlayer;
	if (type == Received_State) {
		// From server to client
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
	} else if (type == Full_State) {
		FullPacket* fp = (FullPacket*)payload;
		if (fp->objectID != 101) { // if 0 is the player itself
			netPlayer = getPlayerBySource(100);
			//thisClient->setNetworkState(fp->fullState.stateID);
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
	} else if (type == String_Message) {
		StringPacket* realPacket = (StringPacket*)payload;
		string msg = realPacket->GetStringFromData();
		if (thisServer != nullptr) {
			vector<string> msgs = splitStringIntoVector(msg);
			if (msgs[0] == "winner") {
				gameIsFinish = true;
				leaderboards.push_back(points);
				leaderboards.push_back(stoi(msgs[1]));
				string scores = "";
				for (int i = 0; i < leaderboards.size(); i++) {
					scores += std::to_string(leaderboards[i]) + " ";
				}
				thisServer->SendGlobalPacket(StringPacket(scores));
			} else {
				g2Points = stoi(msg);
			}
		} else {
			if (msg == "winner") {
				gameIsFinish = true;
			} else {
				if (gameIsFinish) {
					leaderboards.clear();
					vector<string> msgs = splitStringIntoVector(msg);
					for (string s : msgs) {
						if (s != "") leaderboards.push_back(stoi(s));
					}
				} else {
					g2Points = stoi(msg);
				}
			}
		}
	} else if (type == Player_Disconnected) {
		gameIsFinish = true;
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
