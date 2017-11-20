/* Faculty of Information Technology, Brno University of Technology
*  Subject: ISA (Network Applications and Network Administration) - Project 1
*  Description: POP3 Client
*  Date created: October 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: fileman.h - FileManager class definition
*/

#ifndef _FILEMAN__H
#define _FILEMAN__H

#include <string>

class FileManager {
	private:
		int email_count;
		std::string pwd;

	public:
		std::string out_dir;
		std::string messageID;
		std::string messageUIDL = "none";

		bool createOutDir(std::string dir);

		bool saveEmailFile(std::string emailFile, std::string content);

		std::string generateEmailFileByMID(std::string content);
		std::string generateEmailFileByUIDL(std::string uidl);

		bool actualizeTsvFile(std::string uidl, std::string mid);
		bool searchTsvFile(const char * uidl, const char * mid);
};

#endif