/*
 * CommandParser.cpp
 *
 *  Created on: 02-05-2015
 *      Author: mario
 */

#include "IrcProtocolParser.h"

IrcProtocolParser::IrcProtocolParser(Server * server, Client * client) {
	this->server = server;
	this->client = client;
}

void IrcProtocolParser::parse(const string & command) {
	stringstream ss(command);
	vector<string> v;
	while (ss) {
		string part;
		ss >> part;
		v.push_back(part);
	}
	v.pop_back();


	if (v.size() != 0) {
		if (lowerCase(v[0]) == "join") {
			if (v.size() >= 2) {
				for (int i = 1; i < v.size(); ++i) {
					if (!server->joinChannel(v[i], client)) {
						server->createChannel(v[i], "");
						server->joinChannel(v[i], client);
					}
					client->addChannel(v[i]);
					server->sendToChannel(v[i], client->socketDescriptor, client->getPrefix() + "JOIN " + v[i]);
					server->sendToClient(client->getNick(), client->getPrefix() + "JOIN " + v[i]);
					server->sendToClient(client->getNick(), server->getPrefix("332", client) + v[i] + " :" + server->channels[v[i]]->getTopic());
				}
				return;
			} else {
				server->sendToClient(client->socketDescriptor, server->getPrefix("461", client) + "JOIN :Not enough parameters");
				return;
			}
		}

		if (lowerCase(v[0]) == "topic") {
			if (v.size() >= 3) {
				if (server->channels.find(v[1]) != server->channels.end()) {
					if (server->channels[v[1]]->getClients().find(client->getNick()) != server->channels[v[1]]->getClients().end()) {
						if (v[2][0] == ':') {
							for (int i = 3; i < v.size(); ++i) {
								v[2] += " " + v[i];
							}
						}
						server->channels[v[1]]->setTopic(v[2]);
						server->sendToChannel(v[1], client->socketDescriptor, client->getPrefix() + "TOPIC " + v[1] + " " + v[2]);
						server->sendToClient(client->getNick(), client->getPrefix() + "TOPIC " + v[1] + " " + v[2]);
					} else {
						server->sendToClient(client->getNick(), server->getPrefix("442", client) + v[1] + " :You're not on that channel");
					}
				} else {
					server->sendToClient(client->socketDescriptor, server->getPrefix("403", client) + v[1] + " :No such channel");
				}
			} else {
				server->sendToClient(client->getNick(), server->getPrefix("461", client) + "TOPIC :Not enough parameters");
			}
		}

		if (lowerCase(v[0]) == "part") {
			if (v.size() >= 2) {
				for (int i = 1; i < v.size(); ++i) {
					if (server->partChannel(v[i], client)) {
						server->sendToChannel(v[i], client->socketDescriptor, client->getPrefix() + "PART " + v[i]);
						server->sendToClient(client->getNick(), client->getPrefix() + "PART " + v[i]);
						client->removeChannel(v[i]);
					} else {
						server->sendToClient(client->getNick(), server->getPrefix("442", client) + v[i] + " :You're not on that channel");
					}
				}
				return;
			} else {
				server->sendToClient(client->socketDescriptor, server->getPrefix("461", client) + "JOIN :Not enough parameters");
				return;
			}
		}

		if (lowerCase(v[0]) == "privmsg") {
			if (v.size() >= 3) {
				if (v[2][0] == ':') {
					for (int i = 3; i < v.size(); ++i) {
						v[2] += " " + v[i];
					}
				}
				if (v[1][0] == '#') {
					if (server->channels.find(v[1]) != server->channels.end()) {
						Channel * channel = server->channels.find(v[1])->second;
						if (channel->getClients().find(client->getNick()) != channel->getClients().end()) {
							server->sendToChannel(v[1], client->socketDescriptor, client->getPrefix() + command);
						} else {
							server->sendToClient(client->socketDescriptor, server->getPrefix("404", client) + ":Cannot send to channel");
						}
					} else {
						server->sendToClient(client->socketDescriptor, server->getPrefix("403", client) + v[1] + " :No such channel");
					}
				} else {
					if (server->clients.find(v[1]) != server->clients.end()) {
						server->sendToClient(v[1], client->getPrefix() + command);
					} else {
						server->sendToClient(client->socketDescriptor, server->getPrefix("401", client) + v[1] + " :No such nick");
					}
				}
			} else {
				server->sendToClient(client->socketDescriptor, server->getPrefix("412", client) + ":No text to send");
				return;
			}
		}

		if (lowerCase(v[0]) == "list") {
			server->sendToClient(client->socketDescriptor, server->getPrefix("321", client) + "Channel :Users Name");
			if (v.size() == 1) {
				for (map<string, Channel*>::iterator it = server->channels.begin(); it != server->channels.end(); ++it) {
					stringstream ss;
					ss << it->second->getClients().size();
					server->sendToClient(client->socketDescriptor, server->getPrefix("322", client) + it->first + " " + ss.str() + " :" + it->second->getTopic());
				}
			} else {
				for (int i = 1; i < v.size(); ++i) {
					if (server->channels.find(v[i]) != server->channels.end()) {
						Channel * channel = server->channels.find(v[i])->second;
						stringstream ss;
						ss << channel->getClients().size();
						server->sendToClient(client->socketDescriptor, server->getPrefix("322", client) + channel->getName() + " " + ss.str() + " :" + channel->getTopic());
					} else {
						server->sendToClient(client->socketDescriptor, server->getPrefix("322", client) + v[i] + " :No such nick/channel");
					}
				}
			}

			server->sendToClient(client->socketDescriptor, server->getPrefix("323", client) + ":End of /LIST");
			return;
		}

		if (lowerCase(v[0]) == "help") {
			if (v.size() == 1) {
				server->sendToClient(client->socketDescriptor, server->getPrefix("704", client) + "index :Help topics available to users:");
				server->sendToClient(client->socketDescriptor, server->getPrefix("705", client) + "index :");
				server->sendToClient(client->socketDescriptor, server->getPrefix("705", client) + "index :NICK\t\tQUIT\t\tLIST\t\tJOIN");
				server->sendToClient(client->socketDescriptor, server->getPrefix("705", client) + "index :NAMES\t\tWHO\t\tPART\t\tPRIVMSG");
				server->sendToClient(client->socketDescriptor, server->getPrefix("705", client) + "index :HELP\t\tUSER");
				server->sendToClient(client->socketDescriptor, server->getPrefix("705", client) + "index :");
				server->sendToClient(client->socketDescriptor, server->getPrefix("706", client) + "index :End of /HELP.");
				return;
			} else {
				server->sendToClient(client->socketDescriptor, server->getPrefix("524", client) + ":Help not found");
				return;
			}
		}

		if (lowerCase(v[0]) == "who") {

		}

		if (lowerCase(v[0]) == "names") {
			if (v.size() == 1) {
				for (map<string, Channel*>::iterator it = server->channels.begin() ; it != server->channels.end(); ++it) {
					string users;
					for (map<string, Client*>::iterator it2 = it->second->getClients().begin(); it2 != it->second->getClients().end(); ++it2) {
						users += it2->second->getNick() + " ";
					}
					server->sendToClient(client->socketDescriptor, server->getPrefix("353", client) + "= " + it->first + " :" + users);
				}
				server->sendToClient(client->socketDescriptor, server->getPrefix("366", client) + "* :End of /NAMES");
			} else if (server->channels.find(v[1]) != server->channels.end()) {
				Channel * channel = server->channels.find(v[1])->second;
				string users;
				for (map<string, Client*>::iterator it = channel->getClients().begin(); it != channel->getClients().end(); ++it) {
					users += it->second->getNick() + " ";
				}
				server->sendToClient(client->socketDescriptor, server->getPrefix("353", client) + "= " + channel->getName() + " :" + users);
				server->sendToClient(client->socketDescriptor, server->getPrefix("366", client) + v[1] + " :End of /NAMES");
			}

		}

		if (lowerCase(v[0]) == "quit") {
			list<string> channels = client->getChannels();
			for (list<string>::iterator it = channels.begin(); it != channels.end(); ++it) {
				server->sendToChannel(*it, client->socketDescriptor, client->getPrefix() + "QUIT :Client Quit");
			}
			server->quitClient(client);
			return;
		}
	}
}

void IrcProtocolParser::parseNickUser(const string& command) {
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
			if (v.size() >= 2) {
				if (server->clients.find(v[1]) != server->clients.end()) {
					server->sendToClient(client->socketDescriptor, server->getPrefix("433", client) + v[1] + " :Nickname is already in use");
				} else {
					client->setNick(v[1]);
				}
			} else {
				server->sendToClient(client->socketDescriptor, server->getPrefix("431", client) + ":No nickname given");
			}

			return;
		}

		if (lowerCase(v[0]) == "user") {
			if (v.size() >= 5) {
				if (v[4][0] == ':') {
					for (int i = 5; i < v.size(); ++i) {
						v[4] += " " + v[i];
					}
					client->setRealname(v[4].substr(1, v[4].size()-1));
				} else {
					client->setRealname(v[4]);
				}
				client->setUsername(v[1]);

			} else {
				server->sendToClient(client->socketDescriptor, server->getPrefix("461", client) + "USER :Not enough parameters");
			}
			return;
		}
	}

}

string IrcProtocolParser::lowerCase(string in) const {
	transform(in.begin(), in.end(), in.begin(), ::tolower);
	return in;
}


IrcProtocolParser::~IrcProtocolParser() {
}

