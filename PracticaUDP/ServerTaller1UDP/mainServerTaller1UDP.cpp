#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <cstring>

#define MAX_MENSAJES 30

using namespace sf;
using namespace std;

struct Player {
	int posX = -1; 
	int posY = -1;
	int ID = -1;
	IpAddress senderIP;
	unsigned short senderPort;
};

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
	int discID;

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
			if (recType == 0) {
				conn >> mes;
				tempID++;
				player->ID = tempID;
				player->posX = rand() % 10;
				player->posY = rand() % 10;
				ack << player->ID;
				ack << player->posX;
				ack << player->posY;
				socket.send(ack, player->senderIP, player->senderPort);
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
				}
			}
			else if (recType == 1) {
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

		}

	}
	
	system("pause");
	return 0;
}