/*
 * main.cpp
 *
 *  Created on: 29-04-2015
 *      Author: mario
 */

#include <iostream>
#include "Server.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;


int main(int argc, char **argv) {
	Server * server = new Server(6666);
	if (server->isInitializedSocket()) {
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


