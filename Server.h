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
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <map>
#include <list>
#include <pthread.h>
#include <string>
#include "Client.h"
#include "IrcProtocolParser.h"
#include "Channel.h"
#include <iostream>
#include <signal.h>
#include <sstream>
#include <ctime>

using std::string;
using std::stringstream;
using std::map;
using std::list;

class Server {

friend class IrcProtocolParser;

private:
	sockaddr_in serverAddress;
	socklen_t socketLength;
	int serverSocketDescriptor;
	bool initializedSocket;

	string createTime;

	map<string, Channel*> channels;
	map<string, Client*> clients;

	string serverName;
	list<string> motd;

	pthread_t acceptingClients;

	bool initServerSocketDescriptor();
	static void * acceptingClientsLoop(void * arg);
	static void * handleClient(void * arg);

	bool joinChannel(const string & name, Client * client);
	bool partChannel(const string & name, Client * client);
	bool acceptClient();
	string receiveFromClient(int clientDescriptor) const;
	void quitClient(Client * client);

	const string getPrefix(const string & code, Client * client) const;
public:
	Server(int port);
	virtual ~Server();

	bool isInitializedSocket() const {
		return initializedSocket;
	}

	void setServerName(const string & name) {
		serverName = name;
	}
	void setMotd(list<string> & messages) {
		motd = messages;
	}
	void createChannel(const string & name, const string & topic);
	void startAcceptingClients();
	void stopAcceptingClients();
	void stopHandlingClient(Client * client);
	void stopHandlingClients();
	bool sendToClient(int clientDescriptor, const string & message) const;
	bool sendToClient(const string & name, const string & message) const;
	bool sendToChannel(const string & name, int exceptingClientDescriptor, const string & message) const;

	const map<string, Channel*>& getChannels() const {
		return channels;
	}

	const map<string, Client*>& getClients() const {
		return clients;
	}
};

struct ServerClientPair {
	Server * server;
	Client * client;
};

#endif /* SERVER_H_ */
