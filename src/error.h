/* Faculty of Information Technology, Brno University of Technology
*  Subject: ISA (Network Applications and Network Administration) - Project 1
*  Description: POP3 Client
*  Date created: October 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: error.h - header file for Error class
*/

#ifndef _ERROR__H
#define _ERROR__H

enum err_num {

    // argument errors
    SUCCESS_EXIT = 0,
    FLAGS_INCOMPATIBLE = 1,
    FLAG_ALREADY_SET = 2,
    ARGUMENT_ERROR = 3,
    REQUIRED_ARGUMENT = 4,
    INCOMPATIBLE_PORT = 5,
    AUTH_FILE_ERR = 6,
    AUTH_SYNTAX_ERR = 7,
    DIR_NOT_CREATED = 8


};

/*
#include <string>

Class Error {

	private:
		std:string program_name;
		int err_num;

		std::string title;
		std::string content;

	public:
		void error(title, content);
};

*/
#endif