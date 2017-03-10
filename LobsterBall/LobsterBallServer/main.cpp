// SERVER

#include <SFML\Network.hpp>
#include <stdio.h>

int main()
{
	sf::TcpListener listener;
	sf::TcpSocket::Status status;
	sf::Packet packet;
	char msg[] = "Tere t: server\0";

	int port = 53000;

	// bind the listener to a port
	while (listener.listen(port) != sf::Socket::Done)
	{
		continue;
	}
	printf("Server is listening to port: %d\n", port);
	printf("Awaiting connections...\n\n");

	// use "client" to communicate with the connected client,
	// and continue to accept new connections with the listener

	while (1)
	{
		// accept a new connection
		sf::TcpSocket client;
		status = listener.accept(client);
		if (status == sf::Socket::Done)
		{
			printf("Connection received\n");

			// send message to client
			client.send(msg, sizeof(msg));
		}

		if (client.receive(packet))
		{
			// Receive message from client
			printf("Message: %s\n", packet.getData());
		}

		if (client.Disconnected)
		{
			printf("Client disconnected\n");
		}
	}

	return 0;
}