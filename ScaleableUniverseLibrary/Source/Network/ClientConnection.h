#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include "Net.h"

namespace SAU
{
	class ClientConnection : public net::Connection
	{
	public:
		ClientConnection(unsigned int protocolId, float timeout);
		~ClientConnection();

		/*override*/virtual void Update(float deltaTime);
		/*override*/virtual bool SendPacket(const unsigned char data[], int size);
	protected:
		/*override*/virtual bool OnPacketReceived(net::Address& source, const unsigned char data[]);
		/*override*/virtual void OnConnect(std::pair<net::Address, SAU::ReplicationInterface*> connection);
		/*override*/virtual void OnDisconnect(std::pair<net::Address, SAU::ReplicationInterface*> connection);

	private:
		/*override*/virtual void Reset();

		std::pair<net::Address, SAU::ReplicationInterface*> clientConnection;
	};
};
#endif