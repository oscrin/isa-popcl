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

int Pop3Manager::login(Connection con) {

    con.message = "USER " + username + "\r\n";
    con.sendMessage(con.message);

    con.receivedMessage = con.receiveMessage();

    con.message = "PASS " + pwd + "\r\n";
    con.sendMessage(con.message);

    con.receivedMessage = con.receiveMessage();

    return 0;
}

int Pop3Manager::login_SSL(Connection * p_con, std::string CAfile, std::string CApath) {

    p_con->message = "CAPA\r\n";
    p_con->sendMessage();

    p_con->receivedMessage = p_con->receiveMessage();

    p_con->message = "STLS\r\n";
    p_con->sendMessage();

    p_con->receivedMessage = p_con->receiveMessage();

    p_con->prepareSTLS(CAfile.c_str(), CApath.c_str());

//    p_con->message = "CAPA\r\n";
//    p_con->sendMessageSSL(p_con->message);
//    p_con->receivedMessage = p_con->receiveMessageSSL();

    p_con->message = "USER " + username + "\r\n";
    p_con->sendMessageSSL(p_con->message);

    p_con->receivedMessage = p_con->receiveMessageSSL();

    p_con->message = "PASS " + pwd + "\r\n";

    p_con->sendMessageSSL(p_con->message);

    p_con->receivedMessage = p_con->receiveMessageSSL();

    // DOWNLOAD
//	FileManager fm;

	retrieveAllMail(*p_con, "test_folder", true);

   // retrieveMail_SSL(con, fm, 1, "test_folder");

    // LOGOUT

//    logout_SSL(con);

/*    p_con->message = "QUIT\r\n";

	p_con->sendMessageSSL(p_con->message);

	p_con->receiveMessageSSL();

	con.freeSTLS();
*/
    return 0;
}

int Pop3Manager::countMessages(Connection con, bool sslFlag) {

    con.message = "STAT\r\n";

    if (sslFlag) {
	    con.sendMessageSSL(con.message);

	    con.receivedMessage = con.receiveMessageSSL();   
    } else {
	    con.sendMessage(con.message);

	    con.receivedMessage = con.receiveMessage();    	
    }

    std::string s;
    s = con.receivedMessage;
    s.erase(0, s.find(" ") + 1);
    s = s.substr(0, s.find(" "));

    char* pErr = NULL;
    long int l;

    l = strtol(s.c_str(), &pErr, 10);

    messageCount = l;

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

	    check = con.receivedMessage.substr(con.receivedMessage.length()-5,5);

	} while (check.compare("\r\n.\r\n") != 0);

	// remove first line and last characters "\r\n.\r\n"
	content.erase(0, content.find("\r\n") + 2);

	std::string header = content.substr(0,content.find("\r\n\r\n"));
	fm.messageID = getMID(con, header);

	content = dotCorrection(content);
	content.erase(content.length()-5, 5);

	fm.saveEmailFile(fm.messageID, content);
	fm.actualizeTsvFile(fm.messageUIDL, fm.messageID);

//	content = "";

	return 0;
}

int Pop3Manager::retrieveMail_SSL(Connection con, FileManager fm, int mailNum, std::string folder) {

	std::string content = "";
	std::string filename;

	con.message = "RETR " + std::to_string(mailNum) + "\r\n";
	con.sendMessageSSL();

	std::string check;

	do {

	    con.receiveMessageSSL();
	    content.append(con.receivedMessage);

	    check = con.receivedMessage.substr(con.receivedMessage.length()-5,5);

	} while (check.compare("\r\n.\r\n") != 0);

	// remove first line and last characters "\r\n.\r\n"
	content.erase(0, content.find("\r\n") + 2);

	std::string header = content.substr(0,content.find("\r\n\r\n"));
	fm.messageID = getMID(con, header);

	content = dotCorrection(content);
	content.erase(content.length()-5, 5);

	fm.saveEmailFile(fm.messageID, content);
//	fm.actualizeTsvFile(fm.messageUIDL, fm.messageID);

	return 0;
}


int Pop3Manager::retrieveAllMail(Connection con, std::string folder, bool sslFlag) {

	int messageCount = countMessages(con, sslFlag);

	std::string uidl;
	std::string mid;
	FileManager fm;
	fm.createOutDir(folder);

	for (int mailNum = 1; mailNum <= messageCount; mailNum++) {

		if (sslFlag) {
			uidl = getUIDL_SSL(con, mailNum);
		} else {
			uidl = getUIDL(con, mailNum);
		}
		
		fm.messageUIDL = uidl;

		if (sslFlag) {
			retrieveMail_SSL(con, fm, mailNum, folder);			
		}
		else {
			retrieveMail(con, fm, mailNum, folder);
		}
	}

	return messageCount;
}

std::string Pop3Manager::getUIDL(Connection con, int mailNum) {

	con.message = "UIDL " + std::to_string(mailNum) + "\r\n";
	con.sendMessage(con.message);

	con.message = con.receiveMessage();
	std::string uidl = con.message;

	uidl.erase(0,4);
	uidl.erase(0,uidl.find(" ")+1);
	uidl = uidl.substr(0,uidl.length()-2);

//	std::cout << "UIDL: " << uidl << std::endl;

	return uidl;
}

std::string Pop3Manager::getUIDL_SSL(Connection con, int mailNum) {

	con.message = "UIDL " + std::to_string(mailNum) + "\r\n";
	con.sendMessageSSL(con.message);

	con.message = con.receiveMessageSSL();
	std::string uidl = con.message;

	uidl.erase(0,4);
	uidl.erase(0,uidl.find(" ")+1);
	uidl = uidl.substr(0,uidl.length()-2);

//	std::cout << "UIDL: " << uidl << std::endl;

	return uidl;
}

std::string Pop3Manager::getMID(Connection con, std::string header) {

	// Message-Id: <20171107180800.3AFCA6CC@centrum.cz>
	std::string messageID;

	int index = (header.find("Message-Id:") == std::string::npos) ? header.find("Message-ID:") : header.find("Message-Id:");

	if (index == int(std::string::npos)) {

		// TODO - need to be checked
		//	std::cout << "Message-Id not found." << std::endl;
		index = header.find("From:");
		if (index == int(std::string::npos)) {
			messageID = "unidentified" + std::to_string(++unidentified);
			return messageID;
		}

		std::string from = header;
		from.erase(0, index);
		from.erase(0,from.find("<")+1);
		messageID = from.substr(0, from.find(">"));

		index = header.find("Date:");
		if (index == int(std::string::npos)) {
			messageID = "unidentified" + std::to_string(++unidentified);
			return messageID;
		}

		std::string date = header;
		date.erase(0,index);
		date.erase(0,6);
		int i = date.find("\r\n");

		date.erase(i, date.length()-i);
		
		int j = date.find(" ");
		while (j == int(std::string::npos)) {
			date.replace(j,1,"_");
			j = date.find(" ");
		}

		messageID = date + "-" + from;

		return messageID;
	}
	else {

	header.erase(0, index);
	header.erase(0,header.find("<")+1);
	messageID = header.substr(0, header.find(">"));
	}

	return messageID;

}

int Pop3Manager::logout(Connection con) {

	con.message = "QUIT\r\n";
	con.sendMessage();
	con.receiveMessage();
	std::cout << "LOGOUT: " << con.receivedMessage << std::endl;

	return 0;
}

int Pop3Manager::logout_SSL(Connection * con) {

	con->message = "QUIT\r\n";
	con->sendMessageSSL(con->message);
	con->receiveMessageSSL();

	con->freeSTLS();

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
    }
    else {
      std::cerr << "ERROR: Authentication file not found!" << std::endl;
      exit(AUTH_FILE_ERR);
    }

    return true;
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
				/*
					// if after dot there's new line
					if (content_buffer[j] == '\r' && content_buffer[j+1] == '\n') {
				
				*/		// shift all characters left and rewrite second dot
						while (k < (length-2))  {

							content_buffer[k] = content_buffer[k+1];
							k++;	
						}
						// last character set null
						content_buffer[k] = '\0';
				//	}
				}
			}
		}
	}
	return content_buffer;
}