// CLIENT

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>

#include <stdio.h>
#include <iostream>

bool connectToServer(std::string ip);

sf::TcpSocket socket;
sf::Packet packet;
char msg[128] = "Moro t: client\0";

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	std::string ip;

	shape.setFillColor(sf::Color::Green);

	do
	{
		std::cout << "Type ip address: ";
		std::cin >> ip;
		std::cout << "You typed: " << ip << std::endl;
	} while (connectToServer(ip) != true);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			// Receive message from server
			if (socket.receive(packet))
			{
				printf("Message: %s\n", packet.getData());
			}

			if (socket.Disconnected)
			{
				printf("Disconnected\n");
			}
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}

bool connectToServer(std::string ip)
{
	// "192.168.0.8"
	sf::Socket::Status status = socket.connect(ip, 53000);
	if (status != sf::Socket::Done)
	{
		// error...
		printf("Error: could not connect to server\n");
		
		return false;
	}
	else
	{
		printf("Connected!\n");

		// Send message to server
		socket.send(msg, sizeof(msg));

		return true;
	}
}