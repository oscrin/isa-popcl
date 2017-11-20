/* Faculty of Information Technology, Brno University of Technology
*  Subject: ISA (Network Applications and Network Administration) - Project 1
*  Description: POP3 Client
*  Date created: October 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: error.h - Error class definition
*/

#ifndef _ERROR__H
#define _ERROR__H

enum err_num {

    // argument errors
    SUCCESS_EXIT = 0,
    FLAGS_INCOMPATIBLE = 1,
    DOUBLE_ARGUMENT = 2,
    ARGUMENT_ERROR = 3,
    UNKNOWN_ARGUMENT = 11,
    REQUIRED_ARGUMENT = 4,
    INCOMPATIBLE_PORT = 5,
    AUTH_FILE_ERR = 6,
    AUTH_SYNTAX_ERR = 7,
    DIR_NOT_CREATED = 8,
    HOST_UNRESOLVED = 9,
    BAD_LOGIN_OR_PW = -1,
    STLS_NOT_SUPPORTED = -5,
    UNSPECIFIED_INTERNAL_ERROR = -99,
    CONNECTION_ERROR = -31


};

#endif