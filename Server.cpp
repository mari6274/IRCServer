/*
 * Server.cpp
 *
 *  Created on: 29-04-2015
 *      Author: mario
 */

#include "Server.h"

Server::Server(int port) {
	this->serverAddress.sin_port = htons(port);
	this->socketLength = sizeof(serverAddress);
	this->initializedSocket = initServerSocketDescriptor();

}

bool Server::initServerSocketDescriptor() {
	serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == serverSocketDescriptor) return false;
	if (-1 == bind(serverSocketDescriptor, (sockaddr*) &serverAddress, socketLength)) return false;
	if (-1 == listen(serverSocketDescriptor, 100)) return false;
	return true;
}

Server::~Server() {
	close(serverSocketDescriptor);
}

bool Server::acceptClient() {
	Client * client = new Client();
	int desc = accept(serverSocketDescriptor, (sockaddr*) &client->clientAddress, &client->socketLength);
	if (-1 == desc) {
		delete client;
		return false;
	} else if (desc > 0) {
		clients.push_back(client);
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
	server->sendToClient(client->socketDescriptor, "Connected to IRC server.\r\n");

	CommandParser parser;
	while (!client->hasNick() || !client->hasRealname() || !client->hasUsername()) {
		string receivedCommand = pair->server->receiveFromClient(client->socketDescriptor);
		if (!parser.parse(receivedCommand, client)) {
			server->sendToClient(client->socketDescriptor, parser.getError() + "\r\n");
		}
	}
	server->sendToClient(client->socketDescriptor, "Welcome to IRCServer " + client->getNick() + "!\r\n");

	delete pair;
	return NULL;
}

bool Server::sendToClient(int clientDescriptor, string message) const {
	if (-1 == send(clientDescriptor, message.c_str(), message.size(), 0)) {
		return false;
	} else {
		return true;
	}
}

void Server::stopAcceptingClients() {
	pthread_kill(acceptingClients, SIGKILL);
}

void Server::stopHandlingClients() {
	list<Client*>::iterator it;

	for (it = clients.begin(); it != clients.end(); ++it) {
		pthread_kill((*it)->thread, SIGKILL);
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
