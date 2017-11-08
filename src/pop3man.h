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
#include <string>

class Pop3Manager {
	private:
		std::string username;
		std::string pwd;
	public:
		int unsecuredLogin(Connection con);
		bool compileAuthFile(std::string auth_file);

};