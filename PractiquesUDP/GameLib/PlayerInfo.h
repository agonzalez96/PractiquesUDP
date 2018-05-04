#pragma once
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <cstring>
#include <map>


#define MAX_MENSAJES 30

#define SIZE_TABLERO 100
#define LADO_CASILLA 64
#define RADIO_AVATAR 25.f
#define RADIO_COIN 15.f
#define OFFSET_AVATAR 5

using namespace sf;
using namespace std;

struct Player {
	int posX = -1;
	int posY = -1;
	int tmpposX = -1;
	int tmpposY = -1;
	int ID = -1;
	IpAddress senderIP;
	unsigned short senderPort;
	vector<Packet> aMessages;
	map<int, Packet> ackList;
	int IDPacket = 1;
	int ping;
	int score = 0;
	bool win = false;
};

struct Coin {
	int posX = -1;
	int posY = -1;
};