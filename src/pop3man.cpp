/* Faculty of Information Technology, Brno University of Technology
*  Subject: ISA (Network Applications and Network Administration) - Project 1
*  Description: POP3 Client
*  Date created: October 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: pop3man.h - Pop3Manager class implementation
*/

#include "error.h"
#include "pop3man.h"

#include <string>
#include <iostream>
#include <fstream>

int Pop3Manager::unsecuredLogin(Connection con) {
    /* -------------------- SENDING SOCKET -------------------- */

    con.message = "USER " + username + "\r\n";

    con.sendMessage(con.message);

    std::cout << "C: " << con.message;

	/* -------------------- RECEIVING SOCKET -------------------- */

    con.receivedMessage = con.receiveMessage();
   
    std::cout << "S: " << con.receivedMessage;

    /* -------------------- SENDING SOCKET -------------------- */

    con.message = "PASS " + pwd + "\r\n";

    con.sendMessage(con.message);

    std::cout << "C: " << con.message;

	/* -------------------- RECEIVING SOCKET -------------------- */

    con.receivedMessage = con.receiveMessage();
   
    std::cout << "S: " << con.receivedMessage;    

    /* -------------------- SENDING SOCKET -------------------- */
}

bool Pop3Manager::compileAuthFile(std::string auth_file) {
	
	std::ifstream aF;
    std::string nl;

    aF.open(auth_file);
    if (aF.is_open()) {
      getline(aF,username);
      getline(aF,pwd);
      while (!aF.eof()) {
        getline(aF,nl);
        if (!nl.empty()) {
          std::cerr << "ERROR: Syntax error in authentication file." << std::endl;
          exit(AUTH_SYNTAX_ERR); 
        }
      }
   //   std::cout << "Last_line = '" << nl << "'" << std::endl;
      aF.close();
      // auth_parsing
      if (username.substr(0,11).compare("username = ") == 0)
        username.erase(0,11);
      else {
        std::cerr << "ERROR: Syntax error on line 1 in authentication file." << std::endl;
        exit(AUTH_SYNTAX_ERR);
      }
      if (pwd.substr(0,11).compare("password = ") == 0)
        pwd.erase(0,11);
      else {
        std::cerr << "ERROR: Syntax error on line 2 in authentication file." << std::endl;
        exit(AUTH_SYNTAX_ERR);
      }
      // ------------

  //    std::cout << "U: " << username << std::endl << "P: " << pwd << std::endl;
    }
    else {
      std::cerr << "ERROR: Authentication file not found!" << std::endl;
      exit(AUTH_FILE_ERR);
    }
}
