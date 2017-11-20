/* Faculty of Information Technology, Brno University of Technology
*  Subject: ISA (Network Applications and Network Administration) - Project 1
*  Description: POP3 Client
*  Date created: October 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: connection.h - Connection class definition
*/

#ifndef _CON__H
#define _CON__H

#define BUFF_LEN 1024

#include <string>
#include <netinet/in.h>
#include <openssl/ssl.h>

class Connection {
	private:

	public:

        Connection();

		SSL_CTX * ctx_object;
		SSL * ssl_object;

    	char buffer[BUFF_LEN];
		int clientSocket_fd;
    	struct sockaddr_in serverAddr;
        std::string hostname;
    	int portNum;

        long long byteCountSend, byteCountRead = 0;

    	std::string message;
    	std::string receivedMessage;
    	std::string sentMessage;

    	int prepareComunication(int portNum, std::string hostname);
        int prepareSSL(const char * CAfile, const char * CApath);
        int freeSSL();

    	std::string receiveMessage();
    	int sendMessage(std::string message);
    	int sendMessage();

    	std::string receiveMessage_SSL();
    	int sendMessage_SSL(std::string message);
    	int sendMessage_SSL();

		bool isIPv4(const std::string& str);
		bool isIPv6(const std::string& str);
		std::string getIPv4fromHost(std::string hostname);
};

#endif