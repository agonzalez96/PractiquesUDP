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
	int tmpIDPacket, tmpID;

	float secondsPassed = 0.0f;
	float millisPassed = 0.0f;

	Socket::Status status = socket.bind(50000);

	clock_t startTime = clock();
	clock_t msgListStartTime = clock();


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

			//Entra nou jugador
			if (recType == 0) {  

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
					Packet critPack;
					critPack = newInfo;
					critPack << aPlayers[i]->IDPacket;
					aPlayers[i]->IDPacket++;
					socket.send(newInfo, aPlayers[i]->senderIP, aPlayers[i]->senderPort);
					aPlayers[i]->ackList[aPlayers[i]->IDPacket] = newInfo;
			
				}
			}

			//Marxa un jugador
			else if (recType == 1) { 
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
					Packet critPack;
					critPack = newInfo;
					critPack << aPlayers[i]->IDPacket;
					aPlayers[i]->IDPacket++;
					socket.send(newInfo, aPlayers[i]->senderIP, aPlayers[i]->senderPort);
					aPlayers[i]->ackList[aPlayers[i]->IDPacket] = newInfo;
				}
			}

			//Es  mou un jugador
			else if (recType == 2) {  
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
					Packet critPack;
					critPack = newInfo;
					critPack << aPlayers[i]->IDPacket;
					aPlayers[i]->IDPacket++;
					socket.send(newInfo, aPlayers[i]->senderIP, aPlayers[i]->senderPort);
					aPlayers[i]->ackList[aPlayers[i]->IDPacket] = newInfo;
				}
			}
			else if (recType == 3) {
				conn >> tmpID;
				conn >> tmpIDPacket;

				for (int i = 0; i < aPlayers.size(); i++) {
					if (aPlayers[i]->ID == tmpID) {
						aPlayers[i]->ackList.erase(tmpIDPacket);
					}
				}
			}
			else if (recType == 4) {
				conn >> tmpID;
				for (int i = 0; i < aPlayers.size(); i++) {
					if (aPlayers[i]->ID == tmpID) {
						aPlayers[i]->ping = 0;
					}
				}
			}
		}
		else if (status == sf::Socket::NotReady) {
		}

		//Critical Pakcets 
		if (millisPassed > 100) {
			for (int i = 0; i < aPlayers.size(); i++) {
				int sizet = aPlayers[i]->ackList.size();
				if (aPlayers[i]->ackList.size() != 0) {
					for (int j = 0; j < aPlayers[i]->ackList.size(); j++) {
						Packet aux;
						aux = aPlayers[i]->ackList.at(aPlayers[i]->ackList.begin() + j);
						socket.send(aux, aPlayers[i]->senderIP, aPlayers[i]->senderPort);
					}
				}
			}
			msgListStartTime = clock();
			
		}

		//PING
		if (secondsPassed > 1) {
			startTime = clock();
			for (int i = 0; i < aPlayers.size(); i++) {
				Packet _ping;
				sendType = 4;
				_ping << sendType;
				aPlayers[i]->ping++;
			}
		}

		for (int i = 0; i < aPlayers.size(); i++) {
			if (aPlayers[i]->ping >= 60)
				aPlayers.erase(aPlayers.begin() + i);
		}

		secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
		millisPassed = (clock() - msgListStartTime);
		//cout << to_string(millisPassed) << endl;
	}
	
	system("pause");
	return 0;
}