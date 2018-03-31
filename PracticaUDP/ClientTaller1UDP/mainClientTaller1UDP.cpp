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
				aPlayers->push_back(player);
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
		cin >> print;
		if (print == 'p') {
			for (int i = 0; i < aPlayers.size(); i++) {
				cout << "ID: " << aPlayers[i]->ID << endl;
				cout << "Pos x: " << aPlayers[i]->posX << endl;
				cout << "Pos y: " << aPlayers[i]->posY << endl;
			}
		}
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

			Color red(0, 255, 255, 255);
			Color blue(255, 0, 255, 255);

			//Player1
			CircleShape Player1(RADIO_AVATAR);
			Player1.setFillColor(red);
			Vector2f posicionPlayer1(8.f, 8.f);
			posicionPlayer1 = BoardToWindows(posicionPlayer1);
			Player1.setPosition(posicionPlayer1);
			window.draw(Player1);
			//Player2
			for (int i = 1; i < aPlayers.size(); i++) {
				CircleShape Player2 (RADIO_AVATAR);
				Player2.setFillColor(blue);
				Vector2f posicionPlayer2(aPlayers[i]->posX, aPlayers[i]->posX);
				posicionPlayer2 = BoardToWindows(posicionPlayer2);
				Player2.setPosition(posicionPlayer2);
				window.draw(Player2);
			}
			window.display();
		}
	}
	return 0;
}