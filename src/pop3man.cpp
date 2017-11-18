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
#include <cmath>

int Pop3Manager::unsecuredLogin(Connection con) {
    /* -------------------- SENDING SOCKET -------------------- */

    con.message = "USER " + username + "\r\n";

    con.sendMessage(con.message);

//    std::cout << "C: " << con.message;

	/* -------------------- RECEIVING SOCKET -------------------- */

    con.receivedMessage = con.receiveMessage();
   
 //   std::cout << "S: " << con.receivedMessage;

    /* -------------------- SENDING SOCKET -------------------- */

    con.message = "PASS " + pwd + "\r\n";

    con.sendMessage(con.message);

 //   std::cout << "C: " << con.message;

	/* -------------------- RECEIVING SOCKET -------------------- */

    con.receivedMessage = con.receiveMessage();
   
//    std::cout << "S: " << con.receivedMessage;    

    /* -------------------- SENDING SOCKET -------------------- */
    return 0;
}

int Pop3Manager::countMessages(Connection con) {

	/* -------------------- SENDING SOCKET -------------------- */

    con.message = "STAT\r\n";

    con.sendMessage(con.message);

//    std::cout << "C: " << con.message;

	/* -------------------- RECEIVING SOCKET -------------------- */

    con.receivedMessage = con.receiveMessage();
   
//    std::cout << "S: " << con.receivedMessage;

    std::string s;
    s = con.receivedMessage;
    s.erase(0, s.find(" ") + 1);
    s = s.substr(0, s.find(" "));

    char* pErr = NULL;
    long int l;

    l = strtol(s.c_str(), &pErr, 10);

    messageCount = l;

//    std::cout << messageCount << std::endl;

    return messageCount;
}


int Pop3Manager::retrieveMail(Connection con, FileManager fm, int mailNum, std::string folder) {

	std::string content = "";
	std::string filename;

	con.message = "RETR " + std::to_string(mailNum) + "\r\n";
	con.sendMessage();

	std::string check;

	do {

	    con.receiveMessage();
	    content.append(con.receivedMessage);

	//    read_len += con.byteCountRead;

	//    std::cout << "\nREAD: " << read_len << " / " << len << std::endl;

	    check = con.receivedMessage.substr(con.receivedMessage.length()-5,5);

	  //  std::cout << "S: " << receivedMessage;
	} while (check.compare("\r\n.\r\n") != 0);

	// remove first line and last characters "\r\n.\r\n"
	content.erase(0, content.find("\r\n") + 2);

//	filename = std::to_string(i);

	std::string header = content.substr(0,content.find("\r\n\r\n"));

//	std::cout << "HEADER:\n" << header << std::endl;

	content = dotCorrection(content);

	content.erase(content.length()-5, 5);

	filename = fm.generateEmailNameByMID(header);
	fm.saveEmailFile(filename, content);

	fm.actualizeTsvFile(fm.messageUIDL, fm.messageID);

	content = "";


	return 0;

}

char* Pop3Manager::dotCorrection(std::string content) {

	long long length = content.length();
	char * content_buffer = (char*) content.c_str();

	for (int i = 0; i < (length-4); i++) {
		// if end of line
		if(content_buffer[i] == '\r' && content_buffer[i+1] == '\n') {
			// next is dot
			if (content_buffer[i+2] == '.') {

				long long j = i+3;
				long long k = i+3;
				// parsing second and next dots
				while(content_buffer[j] == '.') {
					j++;
					if (j == length-1) {
						return content_buffer;
					}
				}
				// if dots are 2 or more
				if (j > k) {
					// if after dot there's new line
					if (content_buffer[j] == '\r' && content_buffer[j+1] == '\n') {
						// shift all characters left and rewrite second dot
						while (k < (length-2))  {

							content_buffer[k] = content_buffer[k+1];
							k++;	
						}
						// last character set null
						content_buffer[k] = '\0';
					}
				}
			}
		}
	}
	return content_buffer;
}

int Pop3Manager::retrieveMessages(Connection con, std::string folder) {

	
//	std::string s;
	
//	int len = 0;
//	int j;

//	int read_len = 0;

	int messageCount = countMessages(con);

	FileManager fm;
	fm.createOutDir(folder);

	for (int mailNum = 1; mailNum <= messageCount; mailNum++) {

//		mailNum = std::to_string(i);
//
		con.message = "UIDL " + std::to_string(mailNum) + "\r\n";
		con.sendMessage(con.message);
//
		con.message = con.receiveMessage();
		std::string s = con.message;

		std::cout << "UIDL: " << s << std::endl;

		std::string uidl = fm.generateEmailNameByUIDL(s);

	//	s.erase(0, s.find(" ")+1); // skip +OK
	//	std::cout << "TISKNU S2:\n" << s << std::endl;
	//	s.erase(0, s.find(" ")+1); // skip messageNUm
	//	std::cout << "TISKNU S3:\n" << s << std::endl;

	//	char* pErr = NULL;
   	//	long int l;
    //	l = strtol(s.c_str(), &pErr, 10);
    //	len = l;

	//	std::cout << "TISKNU LEN:\n" << len << std::endl;

	//	j = (int) ceil((double) len / BUFF_LEN);



	//	std::cout << "TISKNU CONTENT:\n" << content << std::endl;


//		read_len = 0;
		retrieveMail(con, fm, mailNum, folder);

	}

	return messageCount;
}

int Pop3Manager::logout(Connection con) {

	con.message = "QUIT \r\n";
	con.sendMessage();
	con.receiveMessage();
	std::cout << con.receivedMessage << std::endl;
	return 0;
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
