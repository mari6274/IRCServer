/*
 * CommandParser.h
 *
 *  Created on: 02-05-2015
 *      Author: mario
 */

#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_

#include <string>
#include "Client.h"
#include "Server.h"
#include <algorithm>
#include <sstream>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;
using std::transform;

class Server;

class IrcProtocolParser {
private:
	Server * server;
	Client * client;

	string lowerCase(string in) const;
public:
	void parse(const string & command);
	void parseNickUser(const string & command);

	IrcProtocolParser(Server * server, Client * client);
	virtual ~IrcProtocolParser();
};

#endif /* COMMANDPARSER_H_ */
