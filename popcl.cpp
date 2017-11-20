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
#include "src/help.h"

std::string my_name = "Buddy";

int main(int argc, char* argv[]) {

    Arguments args;

    int result = args.parse(argc,argv);
    if (result == -1) {
    	printHelp();
    	return 0;
    }
    if (result == -2 || result == -3) {
    	return -1;
    }

    Connection con;
    Connection * p_con = &con;
    
    con.portNum = args.getPort();
    con.hostname = args.getServer();

    Pop3Manager p3m;

    int retr;
    int dele;

    if (p3m.compileAuthFile(args.getAuthFile())) {

    	// creates socket, sets IP, port and connects to server
    	con.prepareComunication(con.portNum, con.hostname);

    	if (args.getSFlag()) {
    		p3m.login_STLS(p_con, args.getCAfile(), args.getCApath());
    		retr = p3m.retrieveAllMail(*p_con, args.getOutDir(), true, args.getNFlag());
    		if (args.getDFlag()) {
    			dele = p3m.deleteAllMail(*p_con, true);
    		}
    		p3m.logout_SSL(p_con);
		}
    	else if (args.getTFlag()) {
	    	p3m.login_SSL(p_con, args.getCAfile(), args.getCApath());
	  		retr = p3m.retrieveAllMail(*p_con, args.getOutDir(), true, args.getNFlag());
	  		if (args.getDFlag()) {
    			dele = p3m.deleteAllMail(*p_con, true);
    		}
			p3m.logout_SSL(p_con);
		}
		else {
			p3m.login(con);
			retr = p3m.retrieveAllMail(*p_con, args.getOutDir(), false, args.getNFlag());
			if (args.getDFlag()) {
    			dele = p3m.deleteAllMail(*p_con, false);
    		}
			p3m.logout(con);
		}

		if (args.getNFlag()) {

			if (retr == 0)
				std::cout << "> No new messages found. ";
			else if (retr == 1)
				std::cout << "> " << retr << " new message has been downloaded to your " << args.getOutDir() << ". ";
			else
				std::cout << "> " << retr << " new messages have been downloaded to your " << args.getOutDir() << ". ";	
		}
		else {

			if (retr == 0)
				std::cout << "> There are no messages in your inbox. ";
			else if (retr == 1)
				std::cout << "> " << retr << " message has been downloaded to your " << args.getOutDir() << ". ";
			else
				std::cout << "> " << retr << " messages have been downloaded to your " << args.getOutDir() << ". ";

		}

		if (args.getDFlag()) {

			if (dele == 0)
				std::cout << "No message was deleted from server." << std::endl;
			else if (dele == 1)
				std::cout << dele <<" message has been deleted from serer." << std::endl;
			else
				std::cout << "> " << retr << " messages have been deleted from serer." << std::endl;
		} else {
			std::cout << std::endl;
		}
    }

    return 0;
}