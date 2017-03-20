#include "NetworkTest.h"
#include "Source/Network/ServerConnection.h"
#include "Source/Network/ClientConnection.h"

namespace SAU
{
	bool NetworkTest::initializeSockets()
	{
		return net::InitializeSockets();
	}

	bool NetworkTest::singleConnectionTest()
	{
		printf("-----------------------------------------------------\n");
		printf("test join\n");
		printf("-----------------------------------------------------\n");

		const int ServerPort = 30000;
		const int ClientPort = 30001;
		const int ProtocolId = 0x11112222;
		const float DeltaTime = 0.001f;
		const float TimeOut = 1.5f;

		ClientConnection client(ProtocolId, TimeOut);
		ServerConnection server(ProtocolId, TimeOut);

		bool clientStart = (client.Start(ClientPort));
		bool serverStart = (server.Start(ServerPort));

		if (!clientStart)
		{
			printf("Client Failed to Start\n");
			return false;
		}
		if (!serverStart)
		{
			printf("Server Failed to Start\n");
			return false;
		}

		client.Connect(net::Address(127, 0, 0, 1, ServerPort));
		server.Listen();

		while (true)
		{
			if (client.IsConnected() && server.IsConnected())
				break;

			if (!client.IsConnecting() && client.ConnectFailed())
				break;

			unsigned char client_packet[] = "client to server";
			client.SendPacket(client_packet, sizeof(client_packet));

			unsigned char server_packet[] = "server to client";
			server.SendPacket(server_packet, sizeof(server_packet));

			while (true)
			{
				unsigned char packet[256];
				int bytes_read = client.ReceivePacket(packet, sizeof(packet));
				if (bytes_read == 0)
					break;
			}

			while (true)
			{
				unsigned char packet[256];
				int bytes_read = server.ReceivePacket(packet, sizeof(packet));
				if (bytes_read == 0)
					break;
			}

			client.Update(DeltaTime);
			server.Update(DeltaTime);
		}

		bool clientStillConnected = (client.IsConnected());
		bool serverStillConnected = (server.IsConnected());


		if (!clientStillConnected)
		{
			printf("Client Disconnected\n");
			return false;
		}
		if (!serverStillConnected)
		{
			printf("Server Disconnected\n");
			return false;
		}

		return true;
	}

	bool NetworkTest::multiConnectionTest()
	{
		printf("-----------------------------------------------------\n");
		printf("test join\n");
		printf("-----------------------------------------------------\n");

		const int ServerPort = 30000;
		const int Client1Port = 30001;
		const int Client2Port = 30002;
		const int Client3Port = 30003;
		const int ProtocolId = 0x11112222;
		const float DeltaTime = 0.001f;
		const float TimeOut = 1.5f;

		ClientConnection client1(ProtocolId, TimeOut);
		ClientConnection client2(ProtocolId, TimeOut);
		ClientConnection client3(ProtocolId, TimeOut);
		ServerConnection server(ProtocolId, TimeOut);

		bool client1Start = (client1.Start(Client1Port));
		bool client2Start = (client2.Start(Client2Port));
		bool client3Start = (client3.Start(Client3Port));
		bool serverStart = (server.Start(ServerPort));

		if (!client1Start || !client2Start || !client3Start)
		{
			printf("Client(s) Failed to Start\n");
			return false;
		}
		if (!serverStart)
		{
			printf("Server Failed to Start\n");
			return false;
		}

		client1.Connect(net::Address(127, 0, 0, 1, ServerPort));
		client2.Connect(net::Address(127, 0, 0, 1, ServerPort));
		client3.Connect(net::Address(127, 0, 0, 1, ServerPort));
		server.Listen();

		while (true)
		{
			if (client1.IsConnected() && client2.IsConnected() && client3.IsConnected() && server.IsConnected())
				break;

			if (!client1.IsConnecting() && client1.ConnectFailed() && 
				!client2.IsConnecting() && client2.ConnectFailed() && 
				!client3.IsConnecting() && client3.ConnectFailed())
				break;

			unsigned char client_packet[] = "client to server";
			client1.SendPacket(client_packet, sizeof(client_packet));
			client2.SendPacket(client_packet, sizeof(client_packet));
			client3.SendPacket(client_packet, sizeof(client_packet));

			unsigned char server_packet[] = "server to client";
			server.SendPacket(server_packet, sizeof(server_packet));

			while (true)
			{
				unsigned char packet[256];
				int bytes_read = client1.ReceivePacket(packet, sizeof(packet));
				if (bytes_read == 0)
					break;
			}

			while (true)
			{
				unsigned char packet[256];
				int bytes_read = client2.ReceivePacket(packet, sizeof(packet));
				if (bytes_read == 0)
					break;
			}

			while (true)
			{
				unsigned char packet[256];
				int bytes_read = client3.ReceivePacket(packet, sizeof(packet));
				if (bytes_read == 0)
					break;
			}

			while (true)
			{
				unsigned char packet[256];
				int bytes_read = server.ReceivePacket(packet, sizeof(packet));
				if (bytes_read == 0)
					break;
			}

			client1.Update(DeltaTime);
			client2.Update(DeltaTime);
			client3.Update(DeltaTime);
			server.Update(DeltaTime);
		}

		bool client1StillConnected = (client1.IsConnected());
		bool client2StillConnected = (client2.IsConnected());
		bool client3StillConnected = (client3.IsConnected());
		bool serverStillConnected = (server.IsConnected());


		if (!client1StillConnected || !client2StillConnected || !client3StillConnected)
		{
			printf("Client Disconnected\n");
			return false;
		}
		if (!serverStillConnected)
		{
			printf("Server Disconnected\n");
			return false;
		}

		return true;
	}
}