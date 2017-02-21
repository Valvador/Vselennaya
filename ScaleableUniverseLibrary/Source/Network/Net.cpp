#include "stdafx.h"
#include "ReplicationInterface.h"
#include "Net.h"

namespace net
{
#if PLATFORM == PLATFORM_WINDOWS

	void wait_seconds(float seconds)
	{
		Sleep((int)(seconds * 1000.0f));
	}

#else

#include <unistd.h>
	void wait_seconds(float seconds) { usleep((int)(seconds * 1000000.0f)); }

#endif
	
	void Connection::Listen()
	{
		printf("server listening for connection\n");
		bool connected = IsConnected();
		ClearData();
		mode = Server;
		state = Listening;
	}

	void Connection::Connect(const Address & address)
	{
		printf("client connecting to %d.%d.%d.%d:%d\n",
			address.GetA(), address.GetB(), address.GetC(), address.GetD(), address.GetPort());
		bool connected = IsConnected();
		ClearData();
		mode = Client;
		state = Connecting;

		std::pair<Address, SAU::ReplicationInterface*> serverInterface(address, new SAU::ReplicationInterface(timeout));
		OnConnect(serverInterface);
	}

	void Connection::Update(float deltaTime)
	{
		assert(running);
		std::vector<std::pair<Address, SAU::ReplicationInterface*>> toDisconnect;
		for (auto aMI = addressMap.begin(); aMI != addressMap.end(); aMI++)
		{
			SAU::ReplicationInterface* replicator = aMI->second;
			if (replicator->isTimedOutUpdate(deltaTime))
			{
				// Accumulate items to disconnect at end of Update
				toDisconnect.push_back(*aMI);
			}
			else
			{
				// Call Replicator Updates
				replicator->update();
			}
		}

		for (auto rmRepIt = toDisconnect.begin(); rmRepIt != toDisconnect.end(); rmRepIt++)
		{
			OnDisconnect(*rmRepIt);
		}

		if (addressMap.size() == 0)
		{
			if (state == Connecting)
			{
				printf("connect timed out\n");
				ClearData();
				state = ConnectFail;
			}
			else if (state == Connected)
			{
				printf("connection timed out\n");
				ClearData();
				if (state == Connecting)
					state = ConnectFail;
			}
		}
	}

	// TODO: For now, this sends to EVERY connected client.
	// We need to make this more specific/intelligent.
	bool Connection::SendPacket(Address destination, const unsigned char data[], int size)
	{
		assert(running);
		if (destination.GetAddress() == 0)
			return false;
		unsigned char* packet = new unsigned char[size + 4];
		packet[0] = (unsigned char)(protocolId >> 24);
		packet[1] = (unsigned char)((protocolId >> 16) & 0xFF);
		packet[2] = (unsigned char)((protocolId >> 8) & 0xFF);
		packet[3] = (unsigned char)((protocolId) & 0xFF);
		memcpy(&packet[4], data, size);
		bool res = socket.Send(destination, packet, size + 4);

		delete[] packet;
		return res;
	}

	bool Connection::BroadcastPacket(const unsigned char data[], int size)
	{
		assert(running);
		unsigned char* packet = new unsigned char[size + 4];
		packet[0] = (unsigned char)(protocolId >> 24);
		packet[1] = (unsigned char)((protocolId >> 16) & 0xFF);
		packet[2] = (unsigned char)((protocolId >> 8) & 0xFF);
		packet[3] = (unsigned char)((protocolId) & 0xFF);
		memcpy(&packet[4], data, size);
		bool res = true;
		for (auto aMI = addressMap.begin(); aMI != addressMap.end(); aMI++)
		{
			const Address& address = aMI->first;
			if (address.GetAddress() == 0)
			{
				continue;
			}

			if (!socket.Send(address, packet, size + 4))
			{
				res = false;
			}
		}
		delete[] packet;
		return res;
	}

	int Connection::ReceivePacket(unsigned char data[], int size)
	{
		assert(running);
		unsigned char* packet = new unsigned char[size + 4];
		Address sender;
		int bytes_read = socket.Receive(sender, packet, size + 4);
		if (bytes_read == 0)
		{
			delete[] packet;
			return 0;
		}
		if (bytes_read <= 4)
		{
			delete[] packet;
			return 0;
		}
		if (packet[0] != (unsigned char)(protocolId >> 24) ||
			packet[1] != (unsigned char)((protocolId >> 16) & 0xFF) ||
			packet[2] != (unsigned char)((protocolId >> 8) & 0xFF) ||
			packet[3] != (unsigned char)(protocolId & 0xFF))
		{
			delete[] packet;
			return 0;
		}

		auto addressIt = addressMap.find(sender);
		if (addressIt == addressMap.end())
		{
			if ((addressMap.size() == 0) || (mode == Server))
			{
				OnConnect(std::pair<Address, SAU::ReplicationInterface*>(sender, new SAU::ReplicationInterface(timeout)));
				addressIt = addressMap.find(sender);
			}
		}
		if (addressIt != addressMap.end())
		{
			if (state = Connecting)
			{
				state = Connected;
			}

			memcpy(data, &packet[4], size - 4);
			addressIt->second->processPacket(data);
			delete[] packet;
			return size - 4;
		}
		delete[] packet;
		return 0;
	}

	void Connection::OnConnect(std::pair<Address, SAU::ReplicationInterface*> connection)
	{
		addressMap.insert(connection);
	}

	void Connection::OnDisconnect(std::pair<Address, SAU::ReplicationInterface*> connection)
	{
		assert(addressMap.find(connection.first) != addressMap.end());
		addressMap.erase(connection.first);
		delete connection.second;
	}

	// Private
	void Connection::ClearData()
	{
		state = Disconnected;
		std::map<Address, SAU::ReplicationInterface*> toIterate = addressMap;
		for (auto repIntIt = toIterate.begin(); repIntIt != toIterate.end(); repIntIt++)
		{
			OnDisconnect(*repIntIt);
		}
	}


};