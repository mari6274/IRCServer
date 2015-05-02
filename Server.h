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
#include "Client.h"
#include "CommandParser.h"
#include <iostream>

using std::list;
using std::string;

class Server {
private:
	sockaddr_in serverAddress;
	socklen_t socketLength;
	int serverSocketDescriptor;
	bool initializedSocket;

	list<Client*> clients;

	pthread_t acceptingClients;

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
	bool sendToClient(int clientDescriptor, string message) const;
	string receiveFromClient(int clientDescriptor) const;

};

struct ServerClientPair {
	Server * server;
	Client * client;
};

#endif /* SERVER_H_ */
