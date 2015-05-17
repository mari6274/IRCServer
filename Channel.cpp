/*
 * Channel.cpp
 *
 *  Created on: 02-05-2015
 *      Author: mario
 */

#include "Channel.h"

Channel::Channel(const string & name, const string & topic) {
	this->name = name;
	this->topic = topic;
}

Channel::~Channel() {
}

void Channel::addClient(Client* client) {
	clients[client->getNick()] = client;
}

void Channel::removeClient(Client* client) {
	clients.erase(client->getNick());
}
