#include <PlayerInfo.h>

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

void receiveData(UdpSocket* socket, vector<Player*>* aPlayers, Player* player1) {
	IpAddress senderIP;
	unsigned short senderPort;
	Packet ack;
	string mess;
	int type;
	int count = 0;
	int discID, movID;
	int tempX, tempY;
	int IDPacket;
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
				ack >> IDPacket;
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
					ack >> IDPacket;
					Packet critPack;
					critPack << 3;
					critPack << player->ID;
					critPack << IDPacket;
					socket->send(critPack, "localhost", 50000);
				}
			}

			
			else if (type == 2) {
				ack >> discID;
				for (int i = 0; i < aPlayers->size(); i++) {
					if (aPlayers->at(i)->ID == discID) {
						aPlayers->erase(aPlayers->begin() + i);
						cout << "ID erased: " << aPlayers->at(i)->ID << endl;
					}
				}
				ack >> IDPacket;
				Packet critPack;
				critPack << 3;
				critPack << player->ID;
				critPack << IDPacket;
				socket->send(critPack, "localhost", 50000);
			}
			else if (type == 3) {
				ack >> movID;
				ack >> tempX;
				ack >> tempY;
				for (int i = 0; i < aPlayers->size(); i++) {
					if (aPlayers->at(i)->ID == movID) {
						aPlayers->at(i)->posX = tempX;
						aPlayers->at(i)->posY = tempY;
					}
				}
				ack >> IDPacket;
				Packet critPack;
				critPack << 3;
				critPack << player->ID;
				critPack << IDPacket;
				socket->send(critPack, "localhost", 50000);
			}
			else if (type == 4) {
				Packet ping;
				ping << 4;
				ping << player->ID;
				socket->send(ping, "localhost", 50000);
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


	float secondsPassed = 0.0f;
	clock_t startTime = clock();

	float millisPassed = 0.0f;
	clock_t msgListStartTime = clock();

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
				type = 1;
				disc << type;
				disc << player->ID;
				socket.send(disc, "localhost", 50000);
				break;

			case sf::Event::KeyPressed:
				if (Keyboard::isKeyPressed(Keyboard::A)) {
					if (player->posX > 0) {
						player->posX -= 5;
						break;
					}
				}

				//Right
				if (Keyboard::isKeyPressed(Keyboard::D)) {
					if (player->posX < 587) {
						player->posX += 5;
						break;
					}
				}

				//Up
				if (Keyboard::isKeyPressed(Keyboard::W)) {
					if (player->posY > 0) {
						player->posY -= 5;
						break;
					}
				}

				//Down
				if (Keyboard::isKeyPressed(Keyboard::S)) {
					if (player->posY < 587) {
						player->posY += 5;
						break;
					}
					
				}
				else break;

			default:
				break;

			}
		}
	
		if (millisPassed > 100) {
			Packet mov;
			type = 2;
			mov << type;
			mov << player->ID;
			mov << player->posX;
			mov << player->posY;
			cout << "PosX: " << player->posX << endl;
			cout << "PosY: " << player->posY << endl;
			socket.send(mov, "localhost", 50000);
			msgListStartTime = clock();
		}
		
		window.clear();

		//Draw Players
		CircleShape plCircle(RADIO_AVATAR);

		for (int i = 0; i < aPlayers.size(); ++i) {
		//	plCircle.setPosition(BoardToWindows(Vector2f(aPlayers[i]->posX, aPlayers[i]->posY)));
			plCircle.setPosition(Vector2f(aPlayers[i]->posX, aPlayers[i]->posY));
			if (aPlayers[i]->ID == player->ID) plCircle.setFillColor(sf::Color(255, 0, 0, 255));
			else plCircle.setFillColor(sf::Color(0, 0, 255, 255));
			window.draw(plCircle);
		}
		window.display();

		secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
		millisPassed = (clock() - msgListStartTime);
	}
	t1.join();
	return 0;
}