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

class Connection {
	private:

	public:
    	char buffer[BUFF_LEN];
		int clientSocket;
    	struct sockaddr_in serverAddr;
    	int portNum;
    	std::string message;
    	std::string receivedMessage;
    	std::string hostname;
    	long long byteCountSend, byteCountRead = 0;

    	int prepareComunication(int portNum, std::string hostname);
    	std::string receiveMessage();
    	int sendMessage(std::string message);

		bool isIPv4(const std::string& str);
		bool isIPv6(const std::string& str);
		std::string getIPv4fromHost(std::string hostname);
};

#endif