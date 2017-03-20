#include "stdafx.h"
#include "ClientConnection.h"
#include "ReplicationInterface.h"

namespace SAU
{
	// CONSTRUCTORS/DESTRUCTORS
	ClientConnection::ClientConnection(unsigned int protocolId, float timeout) :
		net::Connection(protocolId, timeout),
		clientConnection(std::pair<net::Address, SAU::ReplicationInterface*>(net::Address(), NULL))
	{
		mode = Client;
	}

	ClientConnection::~ClientConnection()
	{
		Reset();
	}

	// OVERRIDES
	void ClientConnection::Update(float deltaTime)
	{
		assert(running);
		if (clientConnection.second->isTimedOutUpdate(deltaTime))
		{
			// Accumulate items to disconnect at end of Update
			OnDisconnect(clientConnection);
		}
		else
		{ 
			// Call Replicator Updates
			clientConnection.second->update(deltaTime);
		}

		if (clientConnection.second == NULL)
		{
			if (state == Connecting)
			{
				printf("connect timed out\n");
				Reset();
				state = ConnectFail;
			}
			else if (state == Connected)
			{
				printf("connection timed out\n");
				Reset();
				if (state == Connecting)
					state = ConnectFail;
			}
		}
	}

	bool ClientConnection::SendPacket(const unsigned char data[], int size)
	{
		if (clientConnection.second)
		{
			return Connection::SendPacket(clientConnection.first, data, size);
		}
		return false;
	}

	bool ClientConnection::OnPacketReceived(net::Address& source, const unsigned char data[])
	{
		if (clientConnection.first == source)
		{
			if (IsConnecting())
			{
				state = Connected;
			}
			
			clientConnection.second->processPacket(data);
		}

		return false;
	}

	void ClientConnection::OnConnect(std::pair<net::Address, SAU::ReplicationInterface*> connection)
	{
		assert(clientConnection.second == NULL);
		clientConnection = connection;
	}

	void ClientConnection::OnDisconnect(std::pair<net::Address, SAU::ReplicationInterface*> connection)
	{
		if (clientConnection.second && clientConnection.second == connection.second)
		{
			delete clientConnection.second;
			clientConnection = std::pair<net::Address, SAU::ReplicationInterface*>(net::Address(), NULL);
			state = Disconnected;
		}
	}

	void ClientConnection::Reset()
	{
		mode = None;
		OnDisconnect(clientConnection);
	}
};