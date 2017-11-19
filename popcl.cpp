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


int main(int argc, char* argv[]) {


    Arguments args;

    args.parse(argc,argv);

    Connection con;
    
    con.portNum = args.getPort();
    con.hostname = args.getServer();

    std::string receivedMessage;

    Connection * p_con = &con;
        
//    std::cout << "Server = " << args.getServer() << std::endl;

    // creates socket, sets IP, port and connects to server
    con.prepareComunication(con.portNum, con.hostname);
   
    receivedMessage = con.receiveMessage();

    Pop3Manager p3m;

    if (p3m.compileAuthFile(args.getAuthFile())) {
//    	p3m.login(con);
//		p3m.logout(con);

    	p3m.login_SSL(p_con, args.getCAfile(), args.getCApath());
    	p3m.logout_SSL(p_con);
    }

    return 0;
}