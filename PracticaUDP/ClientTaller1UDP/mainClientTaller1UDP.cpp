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
	int ID = 0;
	IpAddress senderIP;
	unsigned short senderPort;
};

void receiveData(UdpSocket* socket, Player* player, vector<Player*>* aPlayers) {
	IpAddress senderIP;
	unsigned short senderPort;
	Packet ack;;
	string mess;
	int type;
	int count = 0;

	while (true) {
		sf::Socket::Status status_r = socket->receive(ack, senderIP, senderPort);

		if (status_r != sf::Socket::Done)
		{
			//std::cout << "failed to receive data" << std::endl;
		}
		else if (status_r == sf::Socket::Disconnected) {
			break;
		}
		else if (status_r == Socket::Done)
		{
			//FUNCIONA
			ack >> type;
			if (type == 0) {
				ack >> mess;
				cout << mess << endl;
				ack >> player->ID;
				cout << "ID: " << player->ID << endl;
			}
			else if (type == 1) {
				//aPlayers->resize(0);
				ack >> player->ID;
				ack >> player->posX;
				ack >> player->posY;
				
				cout << "ID: " << player->ID << endl;
				cout << "posX: " << player->posX << endl;
				cout << "posY: " << player->posY << endl;
			}
			//aMensajes->push_back(buffer);
		}
	}
}

int main()
{
	char print;
	UdpSocket socket;
	Packet conn;
	Player* player = new Player;
	vector<Player*> aPlayers;
	conn << "Holi";
	Socket::Status status = socket.send(conn, "localhost", 50000);
	if (status != Socket::Done) {

	}
	else {

	}

	thread t1(&receiveData, &socket, player, &aPlayers);
	while (true) {
		/*cin >> print;
		if (print == 'p') {
			for (int i = 0; i < aPlayers.size(); i++) {
				cout << "ID: " << aPlayers[i]->ID << endl;
				cout << "Pos x: " << aPlayers[i]->posX << endl;
				cout << "Pos y: " << aPlayers[i]->posY << endl;
			}
		}*/

	}
	return 0;
}