#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <cstring>

#define MAX_MENSAJES 30

#define SIZE_TABLERO 100
#define LADO_CASILLA 64
#define RADIO_AVATAR 25.f
#define OFFSET_AVATAR 5

using namespace sf;
using namespace std;

char tablero[SIZE_TABLERO];

Vector2f TransformaCoordenadaACasilla(int _x, int _y)
{
	float xCasilla = _x / LADO_CASILLA;
	float yCasilla = _y / LADO_CASILLA;
	Vector2f casilla(xCasilla, yCasilla);
	return casilla;
}

Vector2f BoardToWindows(Vector2f _position)
{
	return Vector2f(_position.x*LADO_CASILLA + OFFSET_AVATAR, _position.y*LADO_CASILLA + OFFSET_AVATAR);
}

struct Player {
	int posX = -1;
	int posY = -1;
	int ID = 0;
	IpAddress senderIP;
	unsigned short senderPort;
};

void receiveData(UdpSocket* socket, vector<Player*>* aPlayers, Player* player1) {
	IpAddress senderIP;
	unsigned short senderPort;
	Packet ack;
	string mess;
	int type;
	int count = 0;
	int discID;
	Player* player = new Player;

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
				ack >> player1->ID;
				ack >> player1->posX;
				ack >> player1->posY;
				cout << "ID: " << player->ID << endl;
			}
			else if (type == 1) {
				aPlayers->resize(0);
				ack >> count;
				for (int i = 0; i < count; i++) {
					player = new Player;
					ack >> player->ID;
					ack >> player->posX;
					ack >> player->posY;
					aPlayers->push_back(player);
					cout << "ID: " << player->ID << endl;
					cout << "posX: " << player->posX << endl;
					cout << "posY: " << player->posY << endl;
				}
			}
			else if (type == 2) {
				aPlayers->resize(0);
				count - 1;
				for (int i = 0; i < count; i++) {
					player = new Player;
					ack >> player->ID;
					ack >> player->posX;
					ack >> player->posY;
					aPlayers->push_back(player);
					cout << "ID: " << player->ID << endl;
					cout << "posX: " << player->posX << endl;
					cout << "posY: " << player->posY << endl;
				}
			}
		}
	}
}

int main()
{
	char print;
	UdpSocket socket;
	Packet conn, disc;
	Player* player = new Player;
	vector<Player*> aPlayers;
	int type = 0;
	conn << type;
	conn << "Holi";


	Socket::Status status = socket.send(conn, "localhost", 50000);
	if (status != Socket::Done) {

	}
	else {

	}

	thread t1(&receiveData, &socket, &aPlayers, player);
	
	sf::Vector2f casillaOrigen, casillaDestino;

	sf::RenderWindow window(sf::VideoMode(640, 640), "UDP");

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				disc << 1;
				disc << player->ID;
				status = socket.send(disc, "localhost", 50000);
				break;
			default:
				break;

			}
		}

		window.clear();

		for (int i = 0; i<10; i++)
		{
			for (int j = 0; j<10; j++)
			{
				sf::RectangleShape rectBlanco(sf::Vector2f(LADO_CASILLA, LADO_CASILLA));
				rectBlanco.setFillColor(sf::Color::Black);
				rectBlanco.setOutlineThickness(3);
				rectBlanco.setOutlineColor(sf::Color::White);
				rectBlanco.setPosition(sf::Vector2f(i*LADO_CASILLA, j*LADO_CASILLA));
				window.draw(rectBlanco);
			}
		}

		//Draw Players
		CircleShape plCircle(RADIO_AVATAR);

		for (int i = 0; i < aPlayers.size(); ++i) {
			plCircle.setPosition(BoardToWindows(Vector2f(aPlayers[i]->posX, aPlayers[i]->posY)));
			if (aPlayers[i]->ID == player->ID) plCircle.setFillColor(sf::Color(255, 0, 0, 255));
			else plCircle.setFillColor(sf::Color(0, 0, 255, 255));
			window.draw(plCircle);
		}

		window.display();
	}
	t1.join();
	return 0;
}