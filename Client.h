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

using std::string;

class Client {
private:
	string nick;
	string username;
	string realname;
public:
	sockaddr_in clientAddress;
	socklen_t socketLength;
	int socketDescriptor;
	pthread_t thread;

	Client();
	virtual ~Client();

	Client & operator=(const Client & client);

	const string& getNick() const {
		return nick;
	}

	void setNick(const string& nick) {
		this->nick = nick;
	}

	bool hasNick() const {
		if (nick == "") return false;
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
};

#endif /* CLIENT_H_ */
