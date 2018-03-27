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

void receiveData(TcpSocket* socket, vector<string>* aMensajes) {
	while (true) {
		char buffer[100];
		std::size_t bytesReceived;
		sf::Socket::Status status_r = socket->receive(buffer, sizeof(buffer), bytesReceived);
		if (status_r == Socket::NotReady) {

			continue;
		}
		else if (status_r != sf::Socket::Done)
		{
			std::cout << "failed to receive data" << std::endl;
		}
		else if (status_r == sf::Socket::Disconnected) {
			break;
		}
		else if (status_r == Socket::Done)
		{
			//FUNCIONA
			buffer[bytesReceived] = '\0';
			aMensajes->push_back(buffer);
		}
	}


}

int main()
{
	char data[100];
	std::string textoAEnviar;
	std::cout << "¿Seras servidor (s) o cliente (c)? ... ";
	char c;
	std::cin >> c;
	sf::TcpSocket socket;
	if (c == 's')
	{
		sf::TcpListener listener;
		//5000 es el puerto por el que escucha el servidor.
		//El cliente debe conocer la ip del servidor y el puerto por el que escucha.
		sf::Socket::Status status = listener.listen(50000);
		if (status != sf::Socket::Done)
		{
			//No se puede vincular al puerto 50000
			std::cout << "nope" << std::endl;
		}
		else {
			listener.accept(socket);
			std::string texto = "Conexion con ... " + (socket.getRemoteAddress()).toString() + ":" + std::to_string(socket.getRemotePort()) + "\n";
			std::cout << texto;
			listener.close();
		}
	}
	else if (c == 'c')
	{
		sf::Socket::Status status_sc = socket.connect("localhost", 50000, sf::milliseconds(5.f));
		if (status_sc != sf::Socket::Done) {
			std::cout << "i couldn't connect to server" << std::endl;
		}
		else {
			std::cout << "connected to server" << std::endl;
		}

	}
	else
	{
		exit(0);
	}

	std::vector<string> aMensajes;

	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Chat_Server");

	sf::Font font;
	if (!font.loadFromFile("Champagne & Limousines.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}

	string mensaje = " >";

	sf::Text chattingText(mensaje, font, 14);
	chattingText.setFillColor(sf::Color(0, 160, 0));
	chattingText.setStyle(sf::Text::Bold);


	sf::Text text1(mensaje, font, 14);
	text1.setFillColor(sf::Color(0, 160, 0));
	text1.setStyle(sf::Text::Bold);
	text1.setPosition(0, 560);

	sf::RectangleShape separator(sf::Vector2f(800, 5));
	separator.setFillColor(sf::Color(200, 200, 200, 255));
	separator.setPosition(0, 550);



	//THREAD RECEIVE seguir per aqui

	thread t1(&receiveData, &socket, &aMensajes);

	while (window.isOpen())
	{
		sf::Event evento;
		while (window.pollEvent(evento))
		{
			switch (evento.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (evento.key.code == sf::Keyboard::Escape)
					window.close();
				else if (evento.key.code == sf::Keyboard::Return)
				{
					aMensajes.push_back(mensaje);

					//SEND DEL MESSAGE
					Socket::Status status2 = socket.send(mensaje.c_str(), mensaje.length());

					if (status2 != Socket::Done)
					{
						cout << "Ha fallado el envio de datos" << endl;

					}

					else {
						textoAEnviar = mensaje;
						socket.send(textoAEnviar.c_str(), textoAEnviar.length());
					}


					if (aMensajes.size() > 25)
					{
						aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
					}
					mensaje = ">";
				}
				break;
			case sf::Event::TextEntered:
				if (evento.text.unicode >= 32 && evento.text.unicode <= 126)
					mensaje += (char)evento.text.unicode;
				else if (evento.text.unicode == 8 && mensaje.length() > 0)
					mensaje.erase(mensaje.length() - 1, mensaje.length());
				break;
			}
		}
		window.draw(separator);


		//RECEIVE
		for (size_t i = 0; i < aMensajes.size(); i++)
		{
			std::string chatting = aMensajes[i];
			chattingText.setPosition(sf::Vector2f(0, 20 * i));
			chattingText.setString(chatting);
			window.draw(chattingText);
		}
		std::string mensaje_ = mensaje + "_";
		text1.setString(mensaje_);
		window.draw(text1);
		window.display();
		window.clear();
	}
	socket.disconnect();

	return 0;
}