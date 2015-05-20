/*
 * Client.h
 *
 *  Created on: 01-05-2015
 *      Author: mario
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string>
#include <list>

using std::string;
using std::list;

class Client {
private:
	string nick;
	string username;
	string realname;
	string hostname;

	list<string> channels;
public:
	sockaddr_in clientAddress;
	socklen_t socketLength;
	int socketDescriptor;
	pthread_t thread;

	Client();
	virtual ~Client();

	void addChannel(string name);
	void removeChannel(string name);

	const list<string> & getChannels() const { return channels; }

	Client & operator=(const Client & client);

	string getPrefix() const;

	const string& getNick() const {
		return nick;
	}

	void setNick(const string& nick) {
		this->nick = nick;
	}

	bool hasNick() const {
		if (nick == "" || nick == "*") return false;
		else return true;
	}

	const string& getRealname() const {
		return realname;
	}

	void setRealname(const string& realname) {
		this->realname = realname;
	}

	bool hasRealname() const {
		if (realname == "") return false;
		else return true;
	}

	const string& getUsername() const {
		return username;
	}

	void setUsername(const string& username) {
		this->username = username;
	}

	bool hasUsername() const {
		if (username == "") return false;
		else return true;
	}

	const string& getHostname() const {
		return hostname;
	}

	void setHostname(const string& hostname) {
		this->hostname = hostname;
	}

	bool hasHostname() const {
		if (hostname == "") return false;
		else return true;
	}
};

#endif /* CLIENT_H_ */
