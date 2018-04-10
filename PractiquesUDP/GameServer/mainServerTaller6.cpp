#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <cstring>
#include <PlayerInfo.h>

#define MAX_MENSAJES 30

using namespace sf;
using namespace std;


int main()
{

	srand(time(NULL));

	UdpSocket socket;

	Packet conn;
	string mes;
	vector<Player*> aPlayers;
	int tempID = 0;
	int sendType = 0;
	int recType = 0;
	int discID, movID, movX, movY;

	Socket::Status status = socket.bind(50000);
	while (true) {
		Packet ack;
		sendType = 0;
		ack << sendType;
		ack << "Welcome";
		Player* player = new Player;
		if (status != Socket::Done) {
			cout << "nope";
		}
		else {
			socket.receive(conn, player->senderIP, player->senderPort);
			conn >> recType;
			if (recType == 0) {  //Entra nou jugador
				conn >> mes;
				tempID++;
				player->ID = tempID;
				player->posX = rand() % 10;
				player->posY = rand() % 10;
				for (int i = 0; i < aPlayers.size(); i++) {
					if (aPlayers[i]->posX == player->posX && aPlayers[i]->posY == player->posY) {
						player->posX += 1;
					}
				}
				ack << player->ID;
				ack << player->posX;
				ack << player->posY;
				socket.send(ack, player->senderIP, player->senderPort);
				player->aMessages.push_back(ack);
				aPlayers.push_back(player);

				cout << "IP: " << player->senderIP << endl;
				cout << "Port: " << player->senderPort << endl;
				cout << "mensaje: " << mes << endl;

				Packet newInfo;
				sendType = 1;
				newInfo << sendType;
				newInfo << player->ID;  //Per saber quanta gent hi ha
				for (int j = 0; j < aPlayers.size(); j++) {
					newInfo << aPlayers[j]->ID;
					newInfo << aPlayers[j]->posX;
					newInfo << aPlayers[j]->posY;
				}
				for (int i = 0; i < aPlayers.size(); i++) {
					socket.send(newInfo, aPlayers[i]->senderIP, aPlayers[i]->senderPort);
					aPlayers[i]->aMessages.push_back(newInfo);
				}
			}
			else if (recType == 1) {  //Marxa un jugador
				conn >> discID;
				for (int i = 0; i < aPlayers.size(); i++) {
					if (discID == aPlayers[i]->ID) {
						aPlayers.erase(aPlayers.begin() + i);
					}
				}
				Packet newInfo;
				sendType = 2;
				newInfo << sendType;
				for (int j = 0; j < aPlayers.size(); j++) {
					newInfo << aPlayers[j]->ID;
					newInfo << aPlayers[j]->posX;
					newInfo << aPlayers[j]->posY;
				}
				for (int i = 0; i < aPlayers.size(); i++) {
					socket.send(newInfo, aPlayers[i]->senderIP, aPlayers[i]->senderPort);
				}
			}
			else if (recType == 2) {  //Marxa un jugador
				conn >> movID;
				for (int i = 0; i < aPlayers.size(); i++) {
					if (movID == aPlayers[i]->ID) {
						conn >> movX;
						conn >> movY;
						aPlayers[i]->posX = movX;
						aPlayers[i]->posY = movY;
					}
				}
				Packet newInfo;
				sendType = 3;
				newInfo << sendType;
				for (int j = 0; j < aPlayers.size(); j++) {
					newInfo << aPlayers[j]->ID;
					newInfo << aPlayers[j]->posX;
					newInfo << aPlayers[j]->posY;
				}
				for (int i = 0; i < aPlayers.size(); i++) {
					socket.send(newInfo, aPlayers[i]->senderIP, aPlayers[i]->senderPort);
				}
			}
		}

	}

	system("pause");
	return 0;
}