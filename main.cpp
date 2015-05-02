/*
 * main.cpp
 *
 *  Created on: 29-04-2015
 *      Author: mario
 */

#include <iostream>
#include "Server.h"
#include <signal.h>

using std::cout;
using std::endl;
using std::cin;
using std::string;

Server * server;

void sigint(int signum) {
	server->stopAcceptingClients();
	server->stopHandlingClients();
	delete server;
	exit(0);
}

int main(int argc, char **argv) {
	server = new Server(6666);
	if (server->isInitializedSocket()) {
		signal(SIGINT, sigint);
		cout << "Socket initialized" << endl;
		server->startAcceptingClients();
		string in;
		while (in != "q") {
			cout << " >>> ";
			cin >> in;
		}
	} else {
		cout << "Cannot initialize server. Exit." << endl;
	}
	delete server;
}


