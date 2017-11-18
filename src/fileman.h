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
		std::string out_dir;

	public:
		struct email_param {
			std::string emailFile;
			int messageNum;

			long int size;

			std::string subject;
			std::string date;
			std::string from;
			std::string to;

			std::string content;
		};

		email_param email;
		std::string messageID;
		std::string messageUIDL;

		bool createOutDir(std::string dir);

		bool saveEmailFile(std::string emailFile, std::string content);

		void identificateMail(std::string content);

		std::string generateEmailNameByMID(std::string content);
		std::string generateEmailNameByUIDL(std::string uidl);
		bool actualizeTsvFile(std::string uidl, std::string mid);
};

#endif