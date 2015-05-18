/*
 * Channel.h
 *
 *  Created on: 02-05-2015
 *      Author: mario
 */

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <string>
#include <map>
#include "Client.h"

using std::string;
using std::map;

class Channel {
private:
	string name;
	string topic;
	map<string, Client*> clients;
public:
	Channel(const string & name, const string & topic);
	virtual ~Channel();
	void addClient(Client * client);
	void removeClient(Client * client);

	map<string, Client*>& getClients() {
		return clients;
	}

	const string& getName() const {
		return name;
	}

	const string& getTopic() const {
		return topic;
	}

	void setTopic(const string& topic) {
		this->topic = topic;
	}
};

#endif /* CHANNEL_H_ */
