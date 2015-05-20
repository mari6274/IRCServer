/*
 * Client.cpp
 *
 *  Created on: 01-05-2015
 *      Author: mario
 */

#include "Client.h"

Client::Client() {
}

Client::~Client() {
}

Client& Client::operator =(const Client& client) {
	clientAddress = client.clientAddress;
	socketLength = client.socketLength;
	socketDescriptor = client.socketDescriptor;
	thread = client.thread;
	return *this;
}

void Client::addChannel(string name) {
	channels.push_back(name);
}

void Client::removeChannel(string name) {
	channels.remove(name);
}

string Client::getPrefix() const {
	return ":" + nick + "!" + username + "@" + hostname + " ";
}
