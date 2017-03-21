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
		Reset();

		state = Listening;
	}

	void Connection::Connect(const Address & address)
	{
		printf("client connecting to %d.%d.%d.%d:%d\n",
			address.GetA(), address.GetB(), address.GetC(), address.GetD(), address.GetPort());
		bool connected = IsConnected();
		Reset();
		mode = Client;
		state = Connecting;

		std::pair<Address, SAU::ReplicationInterface*> serverInterface(address, new SAU::ReplicationInterface(timeout));
		OnConnect(serverInterface);
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

		if (!OnPacketSending(destination, data))
		{
			delete[] packet;
			return false;
		}

		memcpy(&packet[4], data, size);
		bool res = socket.Send(destination, packet, size + 4);

		if (OnPacketSent(destination, packet))
		{
			delete[] packet;
			return res;
		}

		delete[] packet;
		return 0;
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

		if (!OnPacketReceiving(sender, packet))
		{
			delete[] packet;
			return 0;
		}

		memcpy(data, &packet[4], size - 4);

		if (OnPacketReceived(sender, data))
		{
			delete[] packet;
			return size - 4;
		}
		delete[] packet;
		return 0;
	}
};