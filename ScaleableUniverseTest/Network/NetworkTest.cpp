#include "NetworkTest.h"
#include "Source/Network/Net.h"
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

	bool NetworkTest::singlePacketTest()
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

		bool serverReceivedPacket = false;
		bool clientReceivedPacket = false;
		unsigned int maxLoops = 500;
		unsigned int loops = 0;

		while ((!serverReceivedPacket || !clientReceivedPacket) && (loops < maxLoops))
		{
			if (client.IsConnected() && server.IsConnected())
				break;

			if (!client.IsConnecting() && client.ConnectFailed())
				break;

			unsigned char client_packet[] = "VerifyClientPacket";
			unsigned int sizeCli = 19;
			client.SendPacket(client_packet, sizeof(client_packet));

			unsigned char server_packet[] = "VerifyServerPacket";
			unsigned int sizeSer = 19;
			server.SendPacket(server_packet, sizeof(server_packet));

			while (true)
			{
				unsigned char packet[256];
				int bytes_read = client.ReceivePacket(packet, sizeof(packet));

				std::string receivedPacket(&packet[0], &packet[sizeSer]);
				std::string serverPacket(&server_packet[0], &server_packet[sizeSer]);
				if (bytes_read == 0)
					break;
				
				if (receivedPacket == serverPacket)
				{
					clientReceivedPacket = true;
					break;
				}
			}

			while (true)
			{
				unsigned char packet[256];
				int bytes_read = server.ReceivePacket(packet, sizeof(packet));

				std::string receivedPacket(&packet[0], &packet[sizeCli]);
				std::string clientPacket(&client_packet[0], &client_packet[sizeCli]);
				if (bytes_read == 0)
					break;

				if (receivedPacket == clientPacket)
				{
					serverReceivedPacket = true;
					break;
				}
			}

			client.Update(DeltaTime);
			server.Update(DeltaTime);
			loops++;
		}

		bool clientStillConnected = (client.IsConnected());
		bool serverStillConnected = (server.IsConnected());


		if (!clientReceivedPacket)
		{
			printf("Client Did Not Get Packet\n");
			return false;
		}
		if (!serverReceivedPacket)
		{
			printf("Server Did Not Get Packet\n");
			return false;
		}
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

	bool NetworkTest::doublePacketTest()
	{
		printf("-----------------------------------------------------\n");
		printf("test join\n");
		printf("-----------------------------------------------------\n");

		const int ServerPort = 30000;
		const int Client1Port = 30001;
		const int Client2Port = 30002;
		const int ProtocolId = 0x11112222;
		const float DeltaTime = 0.001f;
		const float TimeOut = 1.5f;

		ClientConnection client1(ProtocolId, TimeOut);
		ClientConnection client2(ProtocolId, TimeOut);
		ServerConnection server(ProtocolId, TimeOut);

		bool client1Start = (client1.Start(Client1Port));
		bool client2Start = (client2.Start(Client2Port));
		bool serverStart = (server.Start(ServerPort));

		if (!client1Start || !client2Start)
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
		server.Listen();

		bool serverReceivedPacket1 = false;
		bool serverReceivedPacket2 = false;
		bool client1ReceivedPacket = false;
		bool client2ReceivedPacket = false;
		unsigned int maxLoops = 500;
		unsigned int loops = 0;

		while ((!serverReceivedPacket1 || !serverReceivedPacket2 || !client1ReceivedPacket || !client2ReceivedPacket) && (loops < maxLoops))
		{
			if (!client1.IsConnecting() && client1.ConnectFailed())
				break;

			if (!client2.IsConnecting() && client2.ConnectFailed())
				break;

			unsigned char client1_packet[] = "VerifyClientPacket1";
			unsigned char client2_packet[] = "VerifyClientPacket2";
			unsigned int sizeCli = 20;
			client1.SendPacket(client1_packet, sizeof(client1_packet));
			client2.SendPacket(client2_packet, sizeof(client2_packet));

			unsigned char server1_packet[] = "VerifyServerPacket1";
			unsigned char server2_packet[] = "VerifyServerPacket2";
			unsigned int sizeSer = 20;
			server.SendPacket(net::Address(127, 0, 0, 1, Client1Port), server1_packet, sizeof(server1_packet));
			server.SendPacket(net::Address(127, 0, 0, 1, Client2Port), server2_packet, sizeof(server2_packet));

			// Client1 Receive
			while (true)
			{
				unsigned char packet[256];
				int bytes_read = client1.ReceivePacket(packet, sizeof(packet));

				std::string receivedPacket(&packet[0], &packet[sizeSer]);
				std::string serverPacket(&server1_packet[0], &server1_packet[sizeSer]);
				if (bytes_read == 0)
					break;

				if (receivedPacket == serverPacket)
				{
					client1ReceivedPacket = true;
					break;
				}
			}

			// Client2 Receive
			while (true)
			{
				unsigned char packet[256];
				int bytes_read = client2.ReceivePacket(packet, sizeof(packet));

				std::string receivedPacket(&packet[0], &packet[sizeSer]);
				std::string serverPacket(&server2_packet[0], &server2_packet[sizeSer]);
				if (bytes_read == 0)
					break;

				if (receivedPacket == serverPacket)
				{
					client2ReceivedPacket = true;
					break;
				}
			}

			while (true)
			{
				unsigned char packet[256];
				int bytes_read = server.ReceivePacket(packet, sizeof(packet));

				std::string receivedPacket(&packet[0], &packet[sizeCli]);
				std::string client1Packet(&client1_packet[0], &client1_packet[sizeCli]);
				std::string client2Packet(&client2_packet[0], &client2_packet[sizeCli]);
				if (bytes_read == 0)
					break;

				if (receivedPacket == client1Packet)
				{
					serverReceivedPacket1 = true;
				}

				if (receivedPacket == client2Packet)
				{
					serverReceivedPacket2 = true;
				}

				if (serverReceivedPacket1 && serverReceivedPacket2)
					break;
			}

			client1.Update(DeltaTime);
			client2.Update(DeltaTime);
			server.Update(DeltaTime);
			loops++;

			if (client1ReceivedPacket && client2ReceivedPacket && serverReceivedPacket1 && serverReceivedPacket2)
				break;
		}

		bool clientStillConnected = (client1.IsConnected() && client2.IsConnected());
		bool serverStillConnected = (server.IsConnected());


		if (!client1ReceivedPacket)
		{
			printf("Client1 Did Not Get Packet\n");
			return false;
		}
		if (!client2ReceivedPacket)
		{
			printf("Client2 Did Not Get Packet\n");
			return false;
		}
		if (!serverReceivedPacket1)
		{
			printf("Server 1 Did Not Get Packet\n");
			return false;
		}
		if (!serverReceivedPacket2)
		{
			printf("Server 2 Did Not Get Packet\n");
			return false;
		}
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
}