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
#include <string>

using std::list;
using std::string;

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
	list<pthread_t> handlingClientsThreads;

	bool initServerSocketDescriptor();
	static void * acceptingClientsLoop(void * arg);
	static void * handleClient(void * arg);
public:
	Server(int port);
	virtual ~Server();

	bool isInitializedSocket() const {
		return initializedSocket;
	}

	bool acceptClient();
	void startAcceptingClients();
	bool sendToClient(int clientDescriptor, string message);

};

struct ServerClientPair {
	Server * server;
	int clientDesc;
};

#endif /* SERVER_H_ */
