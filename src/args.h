/* Faculty of Information Technology, Brno University of Technology
*  Subject: ISA (Network Applications and Network Administration) - Project 1
*  Description: POP3 Client
*  Date created: October 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: args.h - Arguments class definition
*/

#ifndef _ARGS__H
#define _ARGS__H

#include <string>

// popcl <server> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr>]] [-d] [-n] -a <auth_file> -o <out_dir>

class Arguments {
	private:
        bool server_flag = false;
		std::string server;

		bool p_flag = false;
        int port;

        bool c_flag = false;
        bool C_flag = false;
		bool t_flag = false;	// sets pop3s secured connection
		bool s_flag = false;	// sets non secured connection trought STLS (RFC 2595)

		std::string cert_file = "/dev/null";	// name of certficificate file
		std::string cert_addr = "/dev/null";	// name of folder containing certification files

		bool d_flag = false;	// send server a message for deleting all messages
        bool n_flag = false;    // works only with new messages

        bool a_flag = false;
        bool o_flag = false;

		std::string out_dir;	// sets the path/directory where recieved messages are stored
		std::string auth_file;	// path/file to authentication file

    public:
        Arguments();

        int parse(int argc, char * argv[]);

        void setServer(char* optarg);
        void setPort(char* optarg);

        void setTFlag();
        void setSFlag();

        void setCAfile(char* optarg);
        void setCApath(char* optarg);

        void setDFlag();
        void setNFlag();

        void setOutDir(char* optarg);
        void setAuthFile(char* optarg);

        bool getTFlag();
        bool getSFlag();
        bool getNFlag();
        bool getDFlag();
        
        std::string getServer();
        int getPort();

        std::string getCAfile();
        std::string getCApath();

        std::string getOutDir();
        std::string getAuthFile();

        bool checkArgumentsConsistence();
        bool checkMandatoryArguments();
        int assignPort();
};

#endif