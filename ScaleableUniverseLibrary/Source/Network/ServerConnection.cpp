#include "stdafx.h"

#include "ServerConnection.h"
#include "ReplicationInterface.h"

namespace SAU
{	
	// CONSTRUCTOR/DESTRUCTOR
	ServerConnection::ServerConnection(unsigned int protocolId, float timeout) : 
		Connection(protocolId, timeout)
		
	{
		mode = Server;
	}

	ServerConnection::~ServerConnection()
	{
		Reset();
	}
	
	// OVERRIDES
	// STATE VARIABLE
	bool ServerConnection::IsConnected()
	{
		return (state == Listening && serverConnections.size() > 0);
	}

	// FUNCTIONAL OVERRIDE
	void ServerConnection::Update(float deltaTime)
	{
		assert(running);
		std::vector<std::pair<net::Address, SAU::ReplicationInterface*>> toDisconnect;
		for (auto aMI : serverConnections)
		{
			SAU::ReplicationInterface* replicator = aMI.second;
			if (replicator->isTimedOutUpdate(deltaTime))
			{
				// Accumulate items to disconnect at end of Update
				toDisconnect.push_back(aMI);
			}
			else
			{
				// Call Replicator Updates
				replicator->update(deltaTime);
			}
		}

		for (auto rmRepIt : toDisconnect)
		{
			OnDisconnect(rmRepIt);
		}

		if (serverConnections.size() == 0)
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

	bool ServerConnection::SendPacket(const unsigned char data[], int size)
	{
		bool aPacketWasSent = false;
		for (auto connections : serverConnections)
		{
			if (Connection::SendPacket(connections.first, data, size))
			{
				aPacketWasSent = true;
			}
		}
		return aPacketWasSent;
	}

	bool ServerConnection::OnPacketReceived(net::Address& source, const unsigned char data[])
	{
		if (!IsListening())
			return false;
		
		auto addressIt = serverConnections.find(source);
		if (addressIt == serverConnections.end())
		{					
			OnConnect(std::pair<net::Address, SAU::ReplicationInterface*>(source, new SAU::ReplicationInterface(GetTimeout())));
			addressIt = serverConnections.find(source);
		}

		if (addressIt != serverConnections.end())
		{
			if (IsConnecting())
			{
				state = Connected;
			}
			
			addressIt->second->processPacket(data);
			return true;
		}

		return false;
	}

	void ServerConnection::OnConnect(std::pair<net::Address, SAU::ReplicationInterface*> connection)
	{
		serverConnections.insert(connection);
	}

	void ServerConnection::OnDisconnect(std::pair<net::Address, SAU::ReplicationInterface*> connection)
	{
		assert(serverConnections.find(connection.first) != serverConnections.end());
		serverConnections.erase(connection.first);
		delete connection.second;

		if (serverConnections.size() == 0)
		{
			state = Disconnected;
		}
	}

	// Private
	void ServerConnection::Reset()
	{
		mode = None;
		std::map<net::Address, SAU::ReplicationInterface*> toIterate = serverConnections;
		for (auto repIntIt : toIterate)
		{
			OnDisconnect(repIntIt);
		}
	}
};