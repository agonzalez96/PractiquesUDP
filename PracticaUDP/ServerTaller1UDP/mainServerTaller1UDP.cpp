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
	int type = 0;

	Socket::Status status = socket.bind(50000);
	while (true) {
		Packet ack;
		type = 0;
		ack << type;
		ack << "Welcome";
		Player* player = new Player;
		if (status != Socket::Done) {
			cout << "nope";
		}
		else {
			socket.receive(conn, player->senderIP, player->senderPort);
			conn >> mes;
			tempID++;
			player->ID = tempID;
			player->posX = rand() % 10;
			player->posY = rand() % 10;
			ack << player->ID;
			socket.send(ack, player->senderIP, player->senderPort);
		}
		cout << "IP: " << player->senderIP << endl;
		cout << "Port: " << player->senderPort << endl;
		cout << "mensaje: " << mes << endl;
		aPlayers.push_back(player);
		for (int i = 0; i < aPlayers.size(); i++) {
			for (int j = 0; j < aPlayers.size(); j++) {
				Packet newInfo;
				type = 1;
				newInfo << type;
				newInfo << aPlayers[j]->ID;
	
				newInfo << aPlayers[j]->posX;
				newInfo << aPlayers[j]->posY;
				socket.send(newInfo, aPlayers[i]->senderIP, aPlayers[i]->senderPort);
			}		
		}
	}
	
	system("pause");
	return 0;
}