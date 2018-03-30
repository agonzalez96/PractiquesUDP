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

void receiveData(UdpSocket* socket) {
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
			ack >> mess;
			cout << mess << endl;
			//aMensajes->push_back(buffer);
		}
	}
}

int main()
{
	UdpSocket socket;
	Packet conn;
	conn << "Holi";
	Socket::Status status = socket.send(conn, "localhost", 50000);
	if (status != Socket::Done) {

	}
	else {

	}

	thread t1(&receiveData, &socket);
	while (true) {

	}
	return 0;
}