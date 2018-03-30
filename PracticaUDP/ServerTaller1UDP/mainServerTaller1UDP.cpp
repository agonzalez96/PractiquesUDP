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
	UdpSocket socket;

	Packet conn;
	//ack << "Connected";
	string mes;
	vector<Player*> aPlayers;
	int tempID = 0;

	Socket::Status status = socket.bind(50000);
	while (true) {
		Packet ack;
		Player* player = new Player;
		if (status != Socket::Done) {
			cout << "nope";
		}
		else {
			socket.receive(conn, player->senderIP, player->senderPort);
			conn >> mes;
			tempID++;
			player->ID = tempID;
			ack << player->ID;
			socket.send(ack, player->senderIP, player->senderPort);
		}
		cout << "IP: " << player->senderIP << endl;
		cout << "Port: " << player->senderPort << endl;
		cout << "mensaje: " << mes << endl;
		aPlayers.push_back(player);
	}
	
	system("pause");
	return 0;
}