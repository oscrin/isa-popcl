/* Faculty of Information Technology, Brno University of Technology
*  Subject: ISA (Network Applications and Network Administration) - Project 1
*  Description: POP3 Client
*  Date created: October 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: pop3man.h - Pop3Manager class definition
*/

#include "connection.h"
#include "fileman.h"
#include <string>

class Pop3Manager {
	private:
		std::string username;
		std::string pwd;
		int messageCount;
	public:
		int login(Connection con);
		int loginSSL(Connection con, std::string CAfile, std::string CApath);
		bool compileAuthFile(std::string auth_file);
		int countMessages(Connection con);
		int retrieveMessages(Connection con, std::string folder);
		int retrieveMail(Connection con, FileManager fm, int mailNum, std::string folder);
		int retrieveMailSSL(Connection con, FileManager fm, int mailNum, std::string folder);
		int logout(Connection con);
		int logoutSSL(Connection con);
		char* dotCorrection(std::string content);
};