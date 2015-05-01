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
		args->clientDesc = desc;
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
	pair->server->sendToClient(pair->clientDesc, "Connected to IRC server.");
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
