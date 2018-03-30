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
	int posX, posY, ID;
	IpAddress senderIP;
	unsigned short senderPort;
};

int main()
{
	UdpSocket socket;

	Packet conn, ack;
	ack << "Connected";
	string mes;
	vector<Player> playerPos;
	Socket::Status status = socket.bind(50000);
	while (true) {
		Player* player = new Player;
		if (status != Socket::Done) {
			cout << "nope";
		}
		else {
			socket.receive(conn, player->senderIP, player->senderPort);
			conn >> mes;
			socket.send(ack, player->senderIP, player->senderPort);
		}
		cout << "IP: " << player->senderIP << endl;
		cout << "Port: " << player->senderPort << endl;
		cout << "mensaje: " << mes << endl;
	}
	
	system("pause");
	return 0;
}