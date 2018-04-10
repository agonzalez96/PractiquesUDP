#pragma once
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <cstring>

using namespace sf;
using namespace std;

struct Player {
	int posX = -1;
	int posY = -1;
	int ID = -1;
	IpAddress senderIP;
	unsigned short senderPort;
	vector<Packet> aMessages;
};