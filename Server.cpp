/*
 * Server.cpp
 *
 *  Created on: 29-04-2015
 *      Author: mario
 */

#include "Server.h"

Server::Server(int port) {
	serverName = "IRCServer";
	motd.push_back("hello");
	this->serverAddress.sin_port = htons(port);
	this->socketLength = sizeof(serverAddress);
	this->initializedSocket = initServerSocketDescriptor();
	time_t t = time(0);
	tm * now = localtime( & t );
	createTime = asctime(now);
	createTime.erase(createTime.end()-1);
}

bool Server::initServerSocketDescriptor() {
	serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == serverSocketDescriptor) return false;
	if (-1 == bind(serverSocketDescriptor, (sockaddr*) &serverAddress, socketLength)) return false;
	if (-1 == listen(serverSocketDescriptor, 100)) return false;
	return true;
}

Server::~Server() {
	stopAcceptingClients();
	stopHandlingClients();
	close(serverSocketDescriptor);
}

bool Server::acceptClient() {
	Client * client = new Client();
	client->setNick("*");
	int desc = accept(serverSocketDescriptor, (sockaddr*) &client->clientAddress, &client->socketLength);
	if (-1 == desc) {
		delete client;
		return false;
	} else if (desc > 0) {
		client->socketDescriptor = desc;
		ServerClientPair * args = new ServerClientPair();
		args->server = this;
		args->client = client;
		pthread_create(&client->thread, NULL, handleClient, (void*) args);
//		pthread_join(*handlingClient, NULL);
	}
	return true;

}

void* Server::acceptingClientsLoop(void* arg) {
	Server * server = (Server*) arg;
	while (server->acceptClient());
	return NULL;
}

void Server::startAcceptingClients() {
	pthread_create(&acceptingClients, NULL, acceptingClientsLoop, this);
//	pthread_join(acceptingClients, NULL);
}

void* Server::handleClient(void* arg) {
	ServerClientPair * pair = (ServerClientPair*) arg;
	Server * server = pair->server;
	Client * client = pair->client;

	IrcProtocolParser parser(server, client);
	while (!client->hasNick() || !client->hasRealname() || !client->hasUsername()) {
		string receivedCommand = pair->server->receiveFromClient(client->socketDescriptor);
		parser.parseNickUser(receivedCommand);
	}
	server->clients[client->getNick()] = client;
	server->sendToClient(client->socketDescriptor, server->getPrefix("001", client) + ":Welcome to " + server->serverName + " " + client->getNick() + "!");
	server->sendToClient(client->socketDescriptor, server->getPrefix("003", client) + ":This server was created " + server->createTime + " localtime");
	server->sendToClient(client->socketDescriptor, server->getPrefix("375", client) + ":- <server> Message of the day -");
	for (list<string>::iterator it = server->motd.begin(); it != server->motd.end(); ++it) {
		server->sendToClient(client->socketDescriptor, server->getPrefix("372", client) + ":- " + *it);
	}
	server->sendToClient(client->socketDescriptor, server->getPrefix("376", client) + ":End of /MOTD command");

	while (true) {
		string receivedCommand = pair->server->receiveFromClient(client->socketDescriptor);
		parser.parse(receivedCommand);
	}

	delete pair;
	return NULL;
}

bool Server::sendToClient(int clientDescriptor, const string & message) const {
	string tmp = message;
	tmp += "\r\n";
	if (-1 == send(clientDescriptor, tmp.c_str(), tmp.size(), 0)) {
		return false;
	} else {
		return true;
	}
}


bool Server::sendToClient(const string& name, const string& message) const {
	if (clients.find(name) != clients.end()) {
		Client * client = clients.find(name)->second;
		return sendToClient(client->socketDescriptor, message);
	} else {
		return false;
	}
}

void Server::stopAcceptingClients() {
	pthread_kill(acceptingClients, SIGKILL);
}

void Server::stopHandlingClient(Client* client) {
	pthread_kill(client->thread, SIGKILL);
}

void Server::stopHandlingClients() {
	map<string, Client*>::iterator it;

	for (it = clients.begin(); it != clients.end(); ++it) {
		stopHandlingClient(it->second);
	}
}

void Server::createChannel(const string& name, const string& topic) {
	Channel * channel = new Channel(name, topic);
	channels[name] = channel;
}

bool Server::joinChannel(const string& name, Client * client) {
	if (channels.find(name) != channels.end()) {
		channels[name]->addClient(client);
		return true;
	} else {
		return false;
	}
}

bool Server::partChannel(const string& name, Client* client) {
	if (channels.find(name) != channels.end()) {
		channels[name]->removeClient(client);
		return true;
	} else {
		return false;
	}
}

const string Server::getPrefix(const string & code, Client* client) const {
	stringstream ss;
	ss << ":" << this->serverAddress.sin_addr.s_addr << " " << code << " " << client->getNick() << " ";
	return ss.str();
}

bool Server::sendToChannel(const string & name, int exceptingClientDescriptor,
		const string& message) const {
	if (channels.find(name) != channels.end()) {
		Channel * channel = channels.find(name)->second;
		map<string, Client *> clients = channel->getClients();
		map<string, Client *>::iterator it;
		bool flag = true;
		for (it = clients.begin(); it != clients.end(); ++it) {
			if (it->second->socketDescriptor != exceptingClientDescriptor) {
				if (!sendToClient(it->second->socketDescriptor, message)) {
					flag = false;
				}
			}
		}
		return flag;
	} else {
		return false;
	}
}

string Server::receiveFromClient(int clientDescriptor) const {
	string buffer;
	char c;
	do {
		int retv = recv(clientDescriptor, &c, 1, 0);
		if (-1 != retv) {
			if (c == '\r') {
				recv(clientDescriptor, &c, 1, 0);
				if (c == '\n') {
					break;
				}
			} else {
				buffer += c;
			}
		} else {
			return "";
		}
	} while (true);

	return buffer;
}
