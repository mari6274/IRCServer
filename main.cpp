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
	if (argc == 3) {
		stringstream ss;
		int port;
		ss << argv[2];
		ss >> port;
		if (port != 0) {
			server = new Server(argv[1], port);
			if (server->isInitializedSocket()) {
				signal(SIGINT, sigint);
				cout << "Server is running on " << argv[1] << ":" << port << endl;
				cout << "Quit the server with CONTROL-C" << endl;

				server->createChannel("#SIK", "Kanał uczestników zajęć z Sieci Komputerowych.");

				server->startAcceptingClients();
				string in;
				while (true) {
					cin >> in;
				}
			} else {
				cout << "Cannot initialize server. Exit." << endl;
			}
			sigint(0);
			return 0;
		}
	}

	cout << "Usage: IRCServer <address> <port>" << endl;
}


