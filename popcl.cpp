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

    Pop3Manager p3m;

    if (p3m.compileAuthFile(args.getAuthFile())) {
//    	p3m.login(con);
    	p3m.loginSSL(con, args.getCAfile(), args.getCApath());
    }

//    p3m.logoutSSL(con);

//    con.sendMessage("LIST\r\n");
//    con.receiveMessage();

//    int count = p3m.retrieveMessages(con, args.getOutDir());

/*
    std::cout << "Z main: '" << count << "'" << std::endl;

    con.message = "LIST\r\n";

    con.sendMessage(con.message);

    std::cout << "C: " << con.message;


    receivedMessage = con.receiveMessage();
   
    std::cout << "S: " << receivedMessage; 

    con.message = "RETR 2\r\n";

    con.sendMessage(con.message);

    std::cout << "C: " << con.message;

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

	*/

//	p3m.logout(con);

    return 0;
}