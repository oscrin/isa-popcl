/* Faculty of Information Technology, Brno University of Technology
*  Subject: ISA (Network Applications and Network Administration) - Project 1
*  Description: POP3 Client
*  Date created: October 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: popcl.cpp - client side
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>  // perror()

/*

*/

#include "src/args.h"
#include "src/connection.h"

int main(int argc, char* argv[]) {

    Arguments args;

    args.parse(argc,argv);

    Connection con;

    
    con.portNum = args.getPort();
    con.hostname = args.getServer();
        
   // std::cout << "Server = " << args.getServer() << std::endl;

	/* ------------------- SOCKET DEFINITION -------------------- */

    con.prepareComunication(con.portNum, con.hostname);

    /* -------------------- RECEIVING SOCKET -------------------- */
   
    std::string receivedMessage = con.receiveMessage();
   
    std::cout << receivedMessage;

    /* -------------------- SENDING SOCKET -------------------- */

    con.message = "USER " + args.getUsername() + "\r\n";

    con.sendMessage(con.message);

    std::cout << con.message;

	/* -------------------- RECEIVING SOCKET -------------------- */

    receivedMessage = con.receiveMessage();
   
    std::cout << receivedMessage;

    /* -------------------- SENDING SOCKET -------------------- */

    con.message = "PASS " + args.getPwd() + "\r\n";

    con.sendMessage(con.message);

    std::cout << con.message;

	/* -------------------- RECEIVING SOCKET -------------------- */

    receivedMessage = con.receiveMessage();
   
    std::cout << receivedMessage;    

    /* -------------------- SENDING SOCKET -------------------- */

    con.message = "LIST\r\n";

    con.sendMessage(con.message);

    std::cout << con.message;

	/* -------------------- RECEIVING SOCKET -------------------- */

    receivedMessage = con.receiveMessage();
   
    std::cout << receivedMessage;

    /* -------------------- SENDING SOCKET -------------------- */

    con.message = "RETR 2\r\n";

    con.sendMessage(con.message);

    std::cout << con.message;

	/* -------------------- RECEIVING SOCKET -------------------- */

for (int i = 0; i < 2; i++) {
    receivedMessage = con.receiveMessage();
   
    std::cout << receivedMessage;
}

    return 0;
}