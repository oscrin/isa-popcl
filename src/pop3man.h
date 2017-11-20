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

		bool topFlag;
		bool uidlFlag;

	public:

		Pop3Manager();

		int login(Connection * p_con);
		int login_SSL(Connection * p_con, std::string CAfile, std::string CApath);
		int login_STLS(Connection * p_con, std::string CAfile, std::string CApath);

		bool checkCapabilities(Connection con, std::string capa, bool sslFlag);
		std::string topMail(Connection con, int mailNum, bool sslFlag);
		std::string getMail(Connection con, int mailNum, bool sslFlag);

		int countMessages(Connection con, bool sslFlag);
		std::string getUIDL(Connection con, int mailNum, bool sslFlag);
		std::string getMID(std::string header);

		int retrieveMail(Connection con, FileManager fm, int mailNum, std::string folder, bool sslFlag);	
		int retrieveAllMail(Connection con, std::string folder, bool sslFlag, bool nFlag);
		int deleteAllMail(Connection con, bool sslFlag);
		
		int logout(Connection con);
		int logout_SSL(Connection * con);

		bool compileAuthFile(std::string auth_file);		
		char * dotCorrection(std::string content);
		std::string makeHeaderHash(const char * header);
};