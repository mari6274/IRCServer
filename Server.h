/*
 * Server.h
 *
 *  Created on: 29-04-2015
 *      Author: mario
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <list>
#include <pthread.h>

using std::list;

struct Client {
	sockaddr_in clientAddress;
	socklen_t socketLength;
	int socketDescriptor;
};

class Server {
private:
	sockaddr_in serverAddress;
	socklen_t socketLength;
	int serverSocketDescriptor;
	bool initializedSocket;

	list<Client> clientsDescriptors;

	pthread_t acceptingClients;

	bool initServerSocketDescriptor();
	static void * acceptingClientsLoop(void * arg);
public:
	Server(int port);
	virtual ~Server();

	bool isInitializedSocket() const {
		return initializedSocket;
	}

	bool acceptClient();
	void startAcceptingClients();

};

#endif /* SERVER_H_ */
