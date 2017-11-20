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
#include <iostream>

#include "connection.h"
#include "error.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <openssl/ssl.h>

Connection::Connection() {}

int Connection::prepareComunication(int portNum, std::string hostname) {

	if (isIPv4(hostname)) {
	// Address family = Internet
    serverAddr.sin_family = AF_INET;
    // Set IP address = IPv4 adress
    serverAddr.sin_addr.s_addr = inet_addr(hostname.c_str());
	}
	else if (isIPv6(hostname)) {
    // Address family = Internet6
    serverAddr.sin_family = AF_INET6;
    // Set IP address = IPv6 adress
    serverAddr.sin_addr.s_addr = inet_addr(hostname.c_str());
	}
	else {
	std::string ipv4 = getIPv4fromHost(hostname);
    // Address family = Internet
    serverAddr.sin_family = AF_INET;
    // Set IP address = IPv4 adress got from domain name
    serverAddr.sin_addr.s_addr = inet_addr(ipv4.c_str());
	}

    serverAddr.sin_port = htons(portNum);

    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero)); 

    if ((clientSocket_fd = socket(PF_INET, SOCK_STREAM, 0)) <= 0) {
        perror("ERROR: socket");
        exit(BSD_ERROR);	
    }

    struct timeval t;
    t.tv_sec = 10; 
    t.tv_usec = 0;
    setsockopt(clientSocket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&t,sizeof(struct timeval));

    if (connect(clientSocket_fd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) != 0) {
        perror("ERROR: connect");
        exit(BSD_ERROR);
    }
    
    return 0;
}

// called optionally after prepareCommunication
int Connection::prepareSSL(const char * CAfile, const char * CApath) {

	int status;
	SSL_library_init();

	ctx_object = SSL_CTX_new(SSLv23_client_method());
	if (ctx_object == NULL) {
		std::cerr << "ERROR: CTX object cannot be created." << std::endl;
        exit (SSL_ERROR);
	}

    if (std::string(CApath).compare("/dev/null") == 0 and 
        std::string(CAfile).compare("/dev/null") == 0) {

        SSL_CTX_set_default_verify_paths(ctx_object);

    }
    else {
        // CAfile x CApath potentially NULL
        status = SSL_CTX_load_verify_locations(ctx_object, CAfile, CApath);
        if (status == 0) {
            std::cerr << "ERROR: Certificates not added." << std::endl;
            exit(SSL_ERROR);
        }

    }

	ssl_object = SSL_new(ctx_object);

 	status = SSL_set_fd(ssl_object, clientSocket_fd);
	if (status == 0) {
        std::cerr << "ERROR: SSL object cannot be set to socket file descriptor." << std::endl;
        exit (SSL_ERROR);
	}

	status = SSL_connect(ssl_object);
	if (status <= 0) {
		std::cerr << "> ERROR: The TLS/SSL handshake was unsuccessful." << std::endl;
        exit(SSL_ERROR);
	}

    X509* cert = SSL_get_peer_certificate(ssl_object);
	if ( cert == NULL) {
		// no cert from server
        std::cerr << "ERROR: Server not trustworthy - no certificate found." << std::endl;
        exit(SSL_ERROR);
    }
    else {
        if(SSL_get_verify_result(ssl_object) != X509_V_OK)
        {
            std::cerr << "ERROR: Server not trustworthy - invalid certificate." << std::endl;
            exit(-49);
        }
        else {
            return 0;
        }
    }
}

std::string Connection::receiveMessage() {

	memset(buffer, 0, BUFF_LEN);

    byteCountRead = recv(clientSocket_fd, buffer, BUFF_LEN-1, 0);
        if (byteCountRead == EWOULDBLOCK) {
        std::cout << "Neposlano" << std::endl;
    }
    if (byteCountRead < 0)
        perror("ERROR: recv");

    receivedMessage = buffer;

 //   std::cout << "S: " << receivedMessage;

    return receivedMessage;
}

int Connection::sendMessage(std::string message) {

	memset(buffer, 0, BUFF_LEN);
    strcpy(buffer, message.c_str());

    byteCountSend = send(clientSocket_fd, buffer, message.length(), 0);

    if (byteCountSend < 0)
        perror("ERROR: send");

    sentMessage = message;

 //   std::cout << "C: " << sentMessage;

    return byteCountSend;
}

std::string Connection::receiveMessage_SSL() {

	memset(buffer, 0, BUFF_LEN);

    byteCountRead = SSL_read(ssl_object, buffer, BUFF_LEN-1);
    if (byteCountRead < 0) {
        std::cerr << "ERROR: There are no answers from given server." << std::endl;
        exit(CONNECTION_ERROR);
    }
    receivedMessage = buffer;

//    std::cout << "S(s): " << receivedMessage;

    return receivedMessage;
}

int Connection::sendMessage_SSL(std::string message) {

	memset(buffer, 0, BUFF_LEN);
    strcpy(buffer, message.c_str());

	byteCountSend = SSL_write(ssl_object, buffer, message.length());
	if (byteCountSend < 0)
        perror("ERROR: SSL_write");

    sentMessage = message;

//    std::cout << "C(s): " << sentMessage;

    return byteCountSend;
}


bool Connection::isIPv4(const std::string& str) {
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, str.c_str(), &(sa.sin_addr)) != 0)
    	return true;
    else
    	return false;
}

bool Connection::isIPv6(const std::string& str) {
    struct sockaddr_in6 sa;
    if (inet_pton(AF_INET6, str.c_str(), &(sa.sin6_addr)) != 0)
    	return true;
    else
    	return false; 
}

// zdroj http://beej.us/guide/bgnet/output/html/multipage/gethostbynameman.html
std::string Connection::getIPv4fromHost(std::string hostname) {

	struct hostent *h;
    struct in_addr **addr_list;

	if ((h = gethostbyname(hostname.c_str())) == NULL) {  // get the host info
        std::cerr << "ERROR: Host Unresolved." << std::endl;
        exit(HOST_UNRESOLVED);
    }

    addr_list = (struct in_addr **)h->h_addr_list;

    std::string address = "";
    for (int i = 0; addr_list[i] != NULL; i++) {
        address.append(inet_ntoa(*addr_list[i]));
    }
    // split
    return address;
}

int Connection::freeSSL() {

	SSL_CTX_free(ctx_object);
	SSL_free(ssl_object);
	return 0;
}