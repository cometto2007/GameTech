#include "FinalScreenNetworkingState.h"
#include "../../Common/Assets.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

NCL::CSC8503::FinalScreenNetworkingState::FinalScreenNetworkingState(int time, vector<int> leaderboard, bool isServer)
{
	GameWorld* world = new GameWorld();
	GameTechRenderer* renderer = new GameTechRenderer(*world);
	renderer->setBackgroundColor(Vector4(0.56f, 0.72f, 0.65f, 1));

	newStartTime = clock();
	timerRespawn = time;

	this->finalMenu = new Menu(renderer);
	this->leaderboard = leaderboard;
	this->isServer = isServer;
	writeScoreOnFile();
	readScoreFromFile();
}

void NCL::CSC8503::FinalScreenNetworkingState::OnAwake()
{
}

void NCL::CSC8503::FinalScreenNetworkingState::OnSleep()
{
}

void NCL::CSC8503::FinalScreenNetworkingState::Update()
{
	double secondsPassed = (clock() - newStartTime) / CLOCKS_PER_SEC;
	remTime = (int)(timerRespawn - secondsPassed);
	finalMenu->interact();
	finalMenu->displayMenu(leaderboard, remTime);
}

void NCL::CSC8503::FinalScreenNetworkingState::writeScoreOnFile()
{
	string namefile = "";
	if (isServer) {
		namefile = "serverleaderboard.txt";
	}
	else {
		namefile = "clientleaderboard.txt";
	}
	std::ofstream file(Assets::DATADIR + namefile, std::ios_base::app);
	for (unsigned int i = 0; i < leaderboard.size(); i++)
		file << leaderboard[i] << std::endl;
	file.close();
}

bool NCL::CSC8503::FinalScreenNetworkingState::readScoreFromFile()
{
	string namefile = "";
	if (isServer) {
		namefile = "serverleaderboard.txt";
	} else {
		namefile = "clientleaderboard.txt";
	}
	std::ifstream in(Assets::DATADIR + namefile);
	if (!in) {
		return false;
	}

	string str;
	leaderboard.clear();
	while (std::getline(in, str)) {
		leaderboard.push_back(stoi(str));
	}
	sort(leaderboard.begin(), leaderboard.end(), std::greater<>());

	in.close();
	return true;
}

PushdownState::PushdownResult FinalScreenNetworkingState::PushdownUpdate(PushdownState** pushResult)
{
	Update();
	if (remTime <= 0) {
		return PushdownResult::Pop;
	}
	return PushdownState::NoChange;
}


