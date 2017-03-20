#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include "Net.h"

namespace SAU
{
	class ServerConnection : public net::Connection
	{
	public:
		ServerConnection(unsigned int protocolId, float timeout);
		~ServerConnection();

		// State Variable
		/*override*/virtual bool IsConnected();

		// Functional Overrides
		/*override*/virtual void Update(float deltaTime);
		/*override*/virtual bool SendPacket(const unsigned char data[], int size);
	protected:
		/*override*/virtual bool OnPacketReceived(net::Address& source, const unsigned char data[]);
		/*override*/virtual void OnConnect(std::pair<net::Address, SAU::ReplicationInterface*> connection);
		/*override*/virtual void OnDisconnect(std::pair<net::Address, SAU::ReplicationInterface*> connection);

	private:
		/*override*/virtual void Reset();
		
		// This contains Parent Server, Child Servers, Clients.
		std::map<net::Address, SAU::ReplicationInterface*> serverConnections;
	};

};
#endif