#include <PlayerInfo.h>

#define MAX_MENSAJES 30

using namespace sf;
using namespace std;


int main()
{

	srand(time(NULL));

	UdpSocket socket;

	Packet conn;
	string mes;
	vector<Player*> aPlayers;
	int tempID = 0;
	int sendType = 0;
	int recType = -1;
	int discID, movID, movX, movY;

	float secondsPassed = 0.0f;

	Socket::Status status = socket.bind(50000);

	clock_t startTime = clock();


	socket.setBlocking(false);
	while (true) {
		Packet ack;
		sendType = 0;
		ack << sendType;
		ack << "Welcome";
		Player* player = new Player;
		status = socket.receive(conn, player->senderIP, player->senderPort);
		if (status == sf::Socket::Done) {
			conn >> recType;

			if (recType == 0) {  //Entra nou jugador

				//Creem packet amb la posicio del jugador i fem un send
				conn >> mes;
				tempID++;
				player->ID = tempID;
				player->posX = rand() % 587;
				player->posY = rand() % 587;

				for (int i = 0; i < aPlayers.size(); i++) { //Si la posicio esta ocupada
					if (aPlayers[i]->posX == player->posX && aPlayers[i]->posY == player->posY) {
						player->posX += 100;
						player->posY += 100;
					}
				}
				ack << player->ID;
				ack << player->posX;
				ack << player->posY;
				socket.send(ack, player->senderIP, player->senderPort);


				for (int i = 0; i < aPlayers.size(); i++) { //
					Packet tmpPacket;
					tmpPacket << player->ID;
					tmpPacket << player->posX;
					tmpPacket << player->posY;
				}

				aPlayers.push_back(player);

				cout << "IP: " << player->senderIP << endl;
				cout << "Port: " << player->senderPort << endl;
				cout << "mensaje: " << mes << endl;

				//Fem un packet amb la info de tots els jugadors
				Packet newInfo;
				sendType = 1;
				newInfo << sendType;
				newInfo << player->ID;  //Per saber quanta gent hi ha

				for (int j = 0; j < aPlayers.size(); j++) {
					newInfo << aPlayers[j]->ID;
					newInfo << aPlayers[j]->posX;
					newInfo << aPlayers[j]->posY;
				}
				//Enviem el packet
				for (int i = 0; i < aPlayers.size(); i++) {
					socket.send(newInfo, aPlayers[i]->senderIP, aPlayers[i]->senderPort);
					aPlayers[i]->ackList[aPlayers[i]->IDPacket] = newInfo;
					aPlayers[i]->IDPacket++;

				}
			}


			else if (recType == 1) {  //Marxa un jugador
				conn >> discID;
				for (int i = 0; i < aPlayers.size(); i++) {
					if (discID == aPlayers[i]->ID) {
						aPlayers.erase(aPlayers.begin() + i);
					}
				}
				Packet newInfo;
				sendType = 2;
				newInfo << sendType;
				newInfo << discID;
				for (int i = 0; i < aPlayers.size(); i++) {
					socket.send(newInfo, aPlayers[i]->senderIP, aPlayers[i]->senderPort);
					aPlayers[i]->ackList[aPlayers[i]->IDPacket] = newInfo;
					aPlayers[i]->IDPacket++;
				}
			}


			else if (recType == 2) {  //Es  mou un jugador
				conn >> movID;
				Packet newInfo;
				sendType = 3;
				newInfo << sendType;
				for (int i = 0; i < aPlayers.size(); i++) {
					if (movID == aPlayers[i]->ID) {
						conn >> movX;
						conn >> movY;
						aPlayers[i]->posX = movX;
						aPlayers[i]->posY = movY;
						newInfo << aPlayers[i]->ID;
						newInfo << aPlayers[i]->posX;
						newInfo << aPlayers[i]->posY;
					}
				}
				for (int i = 0; i < aPlayers.size(); i++) {
					socket.send(newInfo, aPlayers[i]->senderIP, aPlayers[i]->senderPort);
					aPlayers[i]->ackList[aPlayers[i]->IDPacket] = newInfo;
					aPlayers[i]->IDPacket++;
				}
			}
		}
		else if (status == sf::Socket::NotReady) {
		}
		if (secondsPassed > 0.01) {
			startTime = clock();
			cout << "holiii";
		}

		secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
	}
	
	system("pause");
	return 0;
}