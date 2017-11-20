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
    HOST_UNRESOLVED = -2,
    STLS_NOT_SUPPORTED = -5,

    FLAGS_INCOMPATIBLE = -11,
    DOUBLE_ARGUMENT = -12,
    ARGUMENT_ERROR = -13,
    REQUIRED_ARGUMENT = -14,
    INCOMPATIBLE_PORT = -15,
    UNKNOWN_ARGUMENT = -16,

    DIR_NOT_CREATED = -21,
    AUTH_FILE_ERR = -22,
    AUTH_SYNTAX_ERR = -23,

    BSD_ERROR = -31,
    SSL_ERROR = -32,

    CONNECTION_ERROR = -42,
    UNSPECIFIED_INTERNAL_ERROR = -99
    


};

#endif