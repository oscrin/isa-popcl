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

int Pop3Manager::login(Connection * p_con) {

	p_con->receiveMessage();

    p_con->sendMessage("USER " + username + "\r\n");

	p_con->receiveMessage();
	if (p_con->receivedMessage.substr(0,4).compare("-ERR") == 0) {
		return -1;
	}

    p_con->sendMessage("PASS " + pwd + "\r\n");

	p_con->receiveMessage();
	if (p_con->receivedMessage.substr(0,4).compare("-ERR") == 0) {
		return -1;
	}

    return 0;
}

int Pop3Manager::login_SSL(Connection * p_con, std::string CAfile, std::string CApath) {

    p_con->prepareSSL(CAfile.c_str(), CApath.c_str());

    p_con->receiveMessage_SSL();

    p_con->sendMessage_SSL("USER " + username + "\r\n");

	p_con->receiveMessage_SSL();
	if (p_con->receivedMessage.substr(0,4).compare("-ERR") == 0) {
		return -1;
	}

    p_con->sendMessage_SSL("PASS " + pwd + "\r\n");

	p_con->receiveMessage_SSL();
	if (p_con->receivedMessage.substr(0,4).compare("-ERR") == 0) {
		return -1;
	}

    return 0;
}

int Pop3Manager::login_STLS(Connection * p_con, std::string CAfile, std::string CApath) {

	p_con->receiveMessage();

    p_con->sendMessage("CAPA\r\n");

	p_con->receiveMessage();
    if ((p_con->receivedMessage).find("STLS") == std::string::npos) {
    	return -2;
    	// TODO Server nepodportuje STLS
    }

    p_con->sendMessage("STLS\r\n");

	p_con->receiveMessage();

    p_con->prepareSSL(CAfile.c_str(), CApath.c_str());

    p_con->sendMessage_SSL("USER " + username + "\r\n");

	p_con->receiveMessage_SSL();
	if (p_con->receivedMessage.substr(0,4).compare("-ERR") == 0) {
		return -1;
	}

    p_con->sendMessage_SSL("PASS " + pwd + "\r\n");

	p_con->receiveMessage_SSL();
	if (p_con->receivedMessage.substr(0,4).compare("-ERR") == 0) {
		return -1;
	}

    return 0;
}

int Pop3Manager::countMessages(Connection con, bool sslFlag) {

    if (sslFlag) {
	    con.sendMessage_SSL("STAT\r\n");
		con.receiveMessage_SSL();   
    } else {
	    con.sendMessage("STAT\r\n");
		con.receiveMessage();    	
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

std::string Pop3Manager::topMail(Connection con, int mailNum, bool sslFlag) {

	std::string header = "";
	std::string check;

	if (sslFlag) {
		con.sendMessage_SSL("TOP " + std::to_string(mailNum) + "0\r\n");
		do {
		    con.receiveMessage_SSL();
		    header.append(con.receivedMessage);
		    check = con.receivedMessage.substr(con.receivedMessage.length()-5,5);
		} while (check.compare("\r\n.\r\n") != 0);
	}
	else {
		con.sendMessage("TOP " + std::to_string(mailNum) + "0\r\n");
		do {
	    	con.receiveMessage();
	    	header.append(con.receivedMessage);
	    	check = con.receivedMessage.substr(con.receivedMessage.length()-5,5);
		} while (check.compare("\r\n.\r\n") != 0);
	}

	// remove first line and last characters "\r\n.\r\n"
	header.erase(0, header.find("\r\n") + 2);
	header.erase(header.length()-5, 5);

	return header;
}

std::string Pop3Manager::getMail(Connection con, int mailNum, bool sslFlag) {

	std::string content = "";
	std::string check;

	if (sslFlag) {
		con.sendMessage_SSL("RETR " + std::to_string(mailNum) + "\r\n");
		do {
		    con.receiveMessage_SSL();
		    content.append(con.receivedMessage);
		    if (con.receivedMessage.length() >=5) {
		    	check = con.receivedMessage.substr(con.receivedMessage.length()-5,5);
		    } else {
		    	break;
		    }
		} while (check.compare("\r\n.\r\n") != 0);
	}
	else {
		con.sendMessage("RETR " + std::to_string(mailNum) + "\r\n");
		do {
	    	con.receiveMessage();
	    	content.append(con.receivedMessage);
	    	if (con.receivedMessage.length() >=5) {
	    		check = con.receivedMessage.substr(con.receivedMessage.length()-5,5);
	    	} else {
	    		break;
	    	}
		} while (check.compare("\r\n.\r\n") != 0);
	}

	// remove first line and last characters "\r\n.\r\n"
	content.erase(0, content.find("\r\n") + 2);
	content.erase(content.length()-5, 5);

	return content;

}

int Pop3Manager::retrieveMail(Connection con, FileManager fm, int mailNum, std::string folder, bool sslFlag) {

	std::string content = "";
	std::string filename;

	content = getMail(con, mailNum, sslFlag);
	if (content.length() == 0) {
		std::cerr << "Content prazdny" << std::endl;
		exit(999);
	}

	std::string header = content.substr(0,content.find("\r\n\r\n"));
	if (header.length() == 0) {
		content = dotCorrection(content);
	} else {
		fm.messageID = getMID(header);
	}

//	content.erase(content.length()-5, 5);
// TODO check dots

	fm.saveEmailFile(fm.messageID, content);

	fm.actualizeTsvFile(fm.messageUIDL, fm.messageID);

	return 0;
}

bool Pop3Manager::checkCapabilities(Connection con, std::string capa, bool sslFlag) {

	if (sslFlag) {
		con.sendMessage_SSL("CAPA\r\n");
		con.receiveMessage_SSL();
	} else {
		con.sendMessage("CAPA\r\n");
		con.receiveMessage();
	}
	if (con.receivedMessage.find(capa) == std::string::npos)
		return false;
	else
		return true;
}

int Pop3Manager::retrieveAllMail(Connection con, std::string folder, bool sslFlag, bool nFlag) {

	uidlFlag = checkCapabilities(con,"UIDL",sslFlag);
	if (!uidlFlag)
		topFlag = checkCapabilities(con,"TOP",sslFlag);

	bool retrFlag = true;

	int messageCount = countMessages(con, sslFlag);
	int retrieved = 0;

	std::string header;
	std::string content;

	FileManager fm;
	fm.createOutDir(folder);

	for (int mailNum = 1; mailNum <= messageCount; mailNum++) {

		if(nFlag) {
			if (uidlFlag) {
				// check according to UIDL
				fm.messageUIDL = getUIDL(con, mailNum, sslFlag);
				retrFlag = !(fm.searchTsvFile(fm.messageUIDL.c_str(), NULL));
			}
			else {
				if (topFlag) {
					// check according to MID in TOP
					header = topMail(con, mailNum, sslFlag);
					fm.messageID = getMID(header);
					retrFlag = !(fm.searchTsvFile(NULL, fm.messageID.c_str()));
				}
				else {
					// check according to MID in RETR
					content = getMail(con, mailNum, sslFlag);
					header = content.substr(0,content.find("\r\n\r\n"));
					fm.messageID = getMID(header);
					retrFlag = !(fm.searchTsvFile(NULL, fm.messageID.c_str()));
				}
			}
		}

		else {
			if (uidlFlag) {
				fm.messageUIDL = getUIDL(con, mailNum, sslFlag);
			}			
		}

		if (retrFlag) {
			retrieveMail(con, fm, mailNum, folder, sslFlag);
			retrieved++;
		}
	}

	return retrieved;
}

int Pop3Manager::deleteAllMail(Connection con, bool sslFlag) {
	
	int messageCount = countMessages(con, sslFlag);
	int dele = 0;

	for (int mailNum = 1; mailNum <= messageCount; mailNum++) {
		if (sslFlag) {
			con.sendMessage_SSL("DELE " + std::to_string(mailNum) + "\r\n");
			con.receiveMessage_SSL();
		}
		else {
			con.sendMessage("DELE " + std::to_string(mailNum) + "\r\n");
			con.receiveMessage();
		}
		dele++;
	}
	return dele;
}

std::string Pop3Manager::getUIDL(Connection con, int mailNum, bool sslFlag) {

	if (sslFlag) {
		con.sendMessage_SSL("UIDL " + std::to_string(mailNum) + "\r\n");
		con.receiveMessage_SSL();		
	} else {
		con.sendMessage("UIDL " + std::to_string(mailNum) + "\r\n");
		con.receiveMessage();
	}

	std::string uidl = con.receivedMessage;

	uidl.erase(0,4);
	uidl.erase(0,uidl.find(" ")+1);
	uidl = uidl.substr(0,uidl.length()-2);

	return uidl;
}

std::string Pop3Manager::getMID(std::string header) {

	// Message-Id: <20171107180800.3AFCA6CC@centrum.cz>
	std::string messageID;

	int index = header.find("Message-Id: ");

	if (index == int(std::string::npos)) {

		index = header.find("Message-ID: ");
	}

	if (index == int(std::string::npos)) {

		std::string hash = makeHeaderHash(header.c_str());

		std::string from;
		index = header.find("From:");

		if (index == int(std::string::npos)) {
			from = "from@unknown.xxx";
		} else {
			from = header;
			from.erase(0, index);
			from.erase(0,from.find("<")+1);
			from = from.substr(0, from.find(">"));
		}

		messageID = hash + "." + from;
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

	con.sendMessage("QUIT\r\n");
	con.receiveMessage();

	return 0;
}

int Pop3Manager::logout_SSL(Connection * con) {

	con->sendMessage_SSL("QUIT\r\n");
	con->receiveMessage_SSL();

	con->freeSSL();

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
	return content_buffer;
}

std::string Pop3Manager::makeHeaderHash(const char * header) {

    // djb2 by Dan Bernstein
    // http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    int c;

    while (c = *header++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return std::to_string(hash);
}