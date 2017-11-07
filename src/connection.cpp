/* Faculty of Information Technology, Brno University of Technology
*  Subject: ISA (Network Applications and Network Administration) - Project 1
*  Description: POP3 Client
*  Date created: October 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: connection.cpp - Connection class implementation
*/

#include <string>
#include <string.h>

#include "connection.h"

#include <arpa/inet.h>
#include <string>
#include <sys/types.h> // connect()
#include <sys/stat.h> // stat()
#include <sys/socket.h> // connect(), send(), bind(), accept()
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iomanip>


int Connection::prepareComunication(int portNum, std::string hostname) {

    if ((clientSocket = socket(PF_INET, SOCK_STREAM, 0)) <= 0) {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(portNum);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr(hostname.c_str());
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero)); 

    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) != 0) {
        perror("ERROR: connect");
        exit(EXIT_FAILURE);
    }
    return 0;
}

std::string Connection::receiveMessage() {

	memset(buffer, 0, BUFF_LEN);

    /*---- Read the message from the server into the buffer ----*/
    byteCountRead = recv(clientSocket, buffer, BUFF_LEN, 0);
    if (byteCountRead < 0)
        perror("ERROR: recvfrom");

    std::string receivedMessage = buffer;
    return receivedMessage;
}


int Connection::sendMessage(std::string message) {

	memset(buffer, 0, BUFF_LEN);
    strcpy(buffer, message.c_str());

    byteCountSend = send(clientSocket, buffer, message.length(), 0);
    if (byteCountSend < 0)
        perror("ERROR: sendto");

    return byteCountSend;
}

bool Connection::isIPv4(const std::string& str) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, str.c_str(), &(sa.sin_addr))!=0;
}

bool Connection::isIPv6(const std::string& str) {
    struct sockaddr_in6 sa;
    return inet_pton(AF_INET6, str.c_str(), &(sa.sin6_addr))!=0;
}

