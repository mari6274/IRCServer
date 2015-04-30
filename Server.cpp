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
	Client client;
	int desc = accept(serverSocketDescriptor, (sockaddr*) &client.clientAddress, &client.socketLength);
	if (-1 == desc) {
		return false;
	} else if (desc > 0) {
		client.socketDescriptor = desc;
		clientsDescriptors.push_back(client);
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
	pthread_join(acceptingClients, NULL);
}
