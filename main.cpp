/*
 * main.cpp
 *
 *  Created on: 29-04-2015
 *      Author: mario
 */

#include <iostream>
#include "Server.h"

using std::cout;

int main(int argc, char **argv) {
	Server * server = new Server(6666);
	if (server->isInitializedSocket()) {
		cout << "Socket initialized";
		server->startAcceptingClients();
	}
	delete server;
}


