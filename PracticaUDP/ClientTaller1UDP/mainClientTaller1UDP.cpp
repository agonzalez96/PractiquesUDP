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

void receiveData(UdpSocket* socket, Player* player) {
	IpAddress senderIP;
	unsigned short senderPort;
	Packet ack;;
	string mess;

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
			//ack >> mess;
			//cout << mess << endl;
			ack >> player->ID;
			cout << "ID: " << player->ID << endl;
			//aMensajes->push_back(buffer);
		}
	}
}

int main()
{
	UdpSocket socket;
	Packet conn;
	Player* player = new Player;
	conn << "Holi";
	Socket::Status status = socket.send(conn, "localhost", 50000);
	if (status != Socket::Done) {

	}
	else {

	}

	thread t1(&receiveData, &socket, player);
	while (true) {

	}
	return 0;
}