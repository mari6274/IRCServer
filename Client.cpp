/*
 * Client.cpp
 *
 *  Created on: 01-05-2015
 *      Author: mario
 */

#include "Client.h"

Client::Client() {
	// TODO Auto-generated constructor stub

}

Client::~Client() {
	// TODO Auto-generated destructor stub
}

Client& Client::operator =(const Client& client) {
	clientAddress = client.clientAddress;
	socketLength = client.socketLength;
	socketDescriptor = client.socketDescriptor;
	thread = client.thread;
	return *this;
}
