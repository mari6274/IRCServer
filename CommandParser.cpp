/*
 * CommandParser.cpp
 *
 *  Created on: 02-05-2015
 *      Author: mario
 */

#include "CommandParser.h"

CommandParser::CommandParser() {
}

bool CommandParser::parse(string command, Client* client) {
	stringstream ss(command);
	vector<string> v;
	while (ss) {
		string part;
		ss >> part;
		v.push_back(part);
	}
	v.pop_back();

	if (v.size() != 0) {
		if (lowerCase(v[0]) == "nick") {
			if (v.size() == 2) {
				client->setNick(v[1]);
				return true;
			} else {
				error = "Invalid parameters count";
				return false;
			}
		}
		if (lowerCase(v[0]) == "user") {
			if (v.size() >= 5) {
				for (int i = 5; i < v.size(); ++i) {
					v[4] += " " + v[i];
				}
				if (
						(v[2] == "0" || v[2] == "8") &&
						v[3] == "*" &&
						v[4][0] == ':'
				) {
					client->setUsername(v[1]);
					client->setRealname(v[4].substr(1, v[4].size()-1));
					return true;
				} else {
					error = "Invalid parameters";
					return false;
				}
			} else {
				error = "Invalid parameters count";
				return false;
			}
		}
		return false;
	} else {
		error = "Empty command";
		return false;
	}

}

string CommandParser::lowerCase(string in) const {
	transform(in.begin(), in.end(), in.begin(), ::tolower);
	return in;
}

CommandParser::~CommandParser() {
}

