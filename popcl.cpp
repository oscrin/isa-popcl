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
#include "src/pop3man.h"
#include "src/fileman.h"

int main(int argc, char* argv[]) {

    Arguments args;

    args.parse(argc,argv);

    Connection con;
    
    con.portNum = args.getPort();
    con.hostname = args.getServer();

    std::string receivedMessage;
        
   // std::cout << "Server = " << args.getServer() << std::endl;

	/* ------------------- SOCKET DEFINITION -------------------- */

    con.prepareComunication(con.portNum, con.hostname); // creates socket, sets IP, port and connects to server

    /* -------------------- RECEIVING SOCKET -------------------- */
   
    receivedMessage = con.receiveMessage();
   
    std::cout << "S: " << receivedMessage;

    Pop3Manager p3m;

    if (p3m.compileAuthFile(args.getAuthFile())) {
    	p3m.unsecuredLogin(con);
    }

    con.message = "LIST\r\n";

    con.sendMessage(con.message);

    std::cout << "C: " << con.message;

	/* -------------------- RECEIVING SOCKET -------------------- */

    receivedMessage = con.receiveMessage();
   
    std::cout << "S: " << receivedMessage;

    /* -------------------- SENDING SOCKET -------------------- */

    con.message = "RETR 2\r\n";

    con.sendMessage(con.message);

    std::cout << "C: " << con.message;

	/* -------------------- RECEIVING SOCKET -------------------- */

	std::string content = "";

for (int i = 0; i < 2; i++) {
    receivedMessage = con.receiveMessage();
   
    content.append(receivedMessage);
    std::cout << "S: " << receivedMessage;
}

	// remove first line and last characters "\r\n.\r\n"
	content.erase(0, content.find("\r\n") + 2);
	content.erase(content.length()-5, content.length());

	std::cout << content;

	FileManager fm;
	fm.createOutDir(args.getOutDir());

	std::string file = fm.generateEmailFileName();

	fm.saveEmailFile(file, content);

    return 0;
}