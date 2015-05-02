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
#include <algorithm>
#include <sstream>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;
using std::transform;

class CommandParser {
private:
	string lowerCase(string in) const;
	string error;
public:
	bool parse(string command, Client * client);

	CommandParser();
	virtual ~CommandParser();

	const string& getError() const {
		return error;
	}
};

#endif /* COMMANDPARSER_H_ */
