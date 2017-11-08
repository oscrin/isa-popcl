/* Faculty of Information Technology, Brno University of Technology
*  Subject: ISA (Network Applications and Network Administration) - Project 1
*  Description: POP3 Client
*  Date created: October 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: fileman.h - FileManager class definition
*/

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
			std::string messageID;
			long int size;

			std::string subject;
			std::string date;
			std::string from;
			std::string to;

			std::string content;
		};

		email_param email;

		bool createOutDir(std::string dir);

		bool saveEmailFile(std::string emailFile, std::string content);

		void identificateMail(std::string content);

		std::string generateEmailFileName();
};