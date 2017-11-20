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

    BAD_LOGIN_OR_PW = -1,
    AUTH_SYNTAX_ERR = -2,
    HOST_UNRESOLVED = -3,
    STLS_NOT_SUPPORTED = -4,

    DOUBLE_ARGUMENT = -11,
    UNKNOWN_ARGUMENT = -12,
    ARGUMENT_ERROR = -13,
    REQUIRED_ARGUMENT = -14,
    INCOMPATIBLE_PORT = -15,

    DIR_NOT_CREATED = -21,
    AUTH_FILE_ERR = -22,

    BSD_ERROR = -31,
    SSL_ERROR = -32,

    UNSPECIFIED_INTERNAL_ERROR = -99,
    CONNECTION_ERROR = -42

};

#endif