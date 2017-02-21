#include "Source/Network/Net.h"
#include "NetworkTest.h"

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

		net::Connection client(ProtocolId, TimeOut);
		net::Connection server(ProtocolId, TimeOut);

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
			client.BroadcastPacket(client_packet, sizeof(client_packet));

			unsigned char server_packet[] = "server to client";
			server.BroadcastPacket(server_packet, sizeof(server_packet));

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

	bool NetworkTest::multiConnectionTest(int numClients)
	{
		return true;
	}
}