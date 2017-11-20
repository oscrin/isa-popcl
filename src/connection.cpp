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
#include <string>
#include <sys/types.h> // connect()
#include <sys/stat.h> // stat()
#include <sys/socket.h> // connect(), send(), bind(), accept()
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iomanip>

#include <openssl/ssl.h>


int Connection::prepareComunication(int portNum, std::string hostname) {

	if (isIPv4(hostname)) {
	/* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr(hostname.c_str());
	}
	else if (isIPv6(hostname)) {
	/* Address family = Internet */
    serverAddr.sin_family = AF_INET6;
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr(hostname.c_str());
	}
	else {
	std::string ipv4 = getIPv4fromHost(hostname);
	/* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr(ipv4.c_str());
	}

    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(portNum);
	/* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero)); 

    if ((clientSocket_fd = socket(PF_INET, SOCK_STREAM, 0)) <= 0) {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);	
    }

    if (connect(clientSocket_fd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) != 0) {
        perror("ERROR: connect");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}

// called after prepareCommunication
int Connection::prepareSSL(const char * CAfile, const char * CApath) {

	int status;
	SSL_library_init();

	ctx_object = SSL_CTX_new(SSLv23_client_method());
	if (ctx_object == NULL) {
		// TODO raise error
		;
	}

	// CAfile x CApath potentially NULL
	status = SSL_CTX_load_verify_locations(ctx_object, CAfile, CApath);
	if (status == 0) {
		// TODO raise error
		// NELZE OVERIT IDENTITU
		;
	}

	if (std::string(CApath).compare("/dev/null") == 0 and 
		std::string(CAfile).compare("/dev/null") == 0) {

	//	std::cout << "Nezadan certifikat" << std::endl;
		SSL_CTX_set_default_verify_paths(ctx_object);
	}

	ssl_object = SSL_new(ctx_object);

 	status = SSL_set_fd(ssl_object, clientSocket_fd);
	if (status == 0) {
		// TODO raise error
		;
	}

	status = SSL_connect(ssl_object);
	if (status <= 0) {
		// TODO raise error
		// The TLS/SSL handshake was not successful
		;
	}

//	long status_l;
//	status_l = SSL_get_verify_result(ssl_object);

//	X509 * cert = 
	if (SSL_get_peer_certificate(ssl_object) == NULL) {
		// server neposlal certifikat
		if(SSL_get_verify_result(ssl_object) != X509_V_OK)
		{
    		/* Handle the failed verification */
    		// prosly certifikat napriklad
    		std::cout << "Neplatny certifikat" << std::endl;
		}

	}

//	X509_free(cert);

	return 0;
}

std::string Connection::receiveMessage() {

	memset(buffer, 0, BUFF_LEN);

    /*---- Read the message from the server into the buffer ----*/
    byteCountRead = recv(clientSocket_fd, buffer, BUFF_LEN-1, 0);
    if (byteCountRead < 0)
        perror("ERROR: recv");

    receivedMessage = buffer;

    std::cout << "S: " << receivedMessage;

    return receivedMessage;
}

int Connection::sendMessage(std::string message) {

	memset(buffer, 0, BUFF_LEN);
    strcpy(buffer, message.c_str());

    byteCountSend = send(clientSocket_fd, buffer, message.length(), 0);
    if (byteCountSend < 0)
        perror("ERROR: send");

    sentMessage = message;

    std::cout << "C: " << sentMessage;

    return byteCountSend;
}

std::string Connection::receiveMessage_SSL() {

	memset(buffer, 0, BUFF_LEN);

    byteCountRead = SSL_read(ssl_object, buffer, BUFF_LEN-1);
    if (byteCountRead < 0)
        perror("ERROR: SSL_read");

    receivedMessage = buffer;

    std::cout << "S(s): " << receivedMessage;

    return receivedMessage;
}

int Connection::sendMessage_SSL(std::string message) {

	memset(buffer, 0, BUFF_LEN);
    strcpy(buffer, message.c_str());

	byteCountSend = SSL_write(ssl_object, buffer, message.length());
	if (byteCountSend < 0)
        perror("ERROR: SSL_write");

    sentMessage = message;

    std::cout << "C(s): " << sentMessage;

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

// TODO zdroj http://beej.us/guide/bgnet/output/html/multipage/gethostbynameman.html
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

    //TODO split
    return address;
}

int Connection::freeSSL() {

	SSL_CTX_free(ctx_object);
	SSL_free(ssl_object);
	return 0;
}