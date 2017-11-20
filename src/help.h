#ifndef _HELP__H
#define _HELP__H

void printHelp() {
    std::cout << "\e[32mProgram usage:" << std::endl;
    std::cout << "\e[31mpopcl <server> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr>]] [-d] [-n] -a <auth_file> -o <out_dir>" << std::endl;
    std::cout << "\e[39m\e[2m     -p   port            - sets remote TCP port" << std::endl;
    std::cout << "     -T   pop3s           - sets pop3s secured connection" << std::endl;
    std::cout << "     -S   STSL            - connects with STLS (RFC 2595)" << std::endl;
    std::cout << "     -c   certfile        - path to certificate file" << std::endl;
    std::cout << "     -C   certfolder      - path to folder with certificates" << std::endl;
    std::cout << "     -d   delete          - deletes all messages from server" << std::endl;
    std::cout << "     -n   new messages    - works only with new messages" << std::endl;
    std::cout << "     -a   authfile        - path to authentication file" << std::endl;
    std::cout << "     -o   outdir          - folder where messages will be stored\e[0m" << std::endl;
}


#endif