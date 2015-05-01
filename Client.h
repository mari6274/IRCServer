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

class Client {
public:
	sockaddr_in clientAddress;
	socklen_t socketLength;
	int socketDescriptor;
	pthread_t thread;

	Client();
	virtual ~Client();

	Client & operator=(const Client & client);
};

#endif /* CLIENT_H_ */
