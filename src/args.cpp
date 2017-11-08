/* Faculty of Information Technology, Brno University of Technology
*  Subject: ISA (Network Applications and Network Administration) - Project 1
*  Description: POP3 Client
*  Date created: October 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: args.cpp - Argument class implementation
*/

// popcl <server> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr>]] [-d] [-n] -a <auth_file> -o <out_dir>

#include "args.h"
#include "error.h"

#include <iostream>
#include <stdlib.h>
#include <unistd.h>


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
    std::cout << "     -a   authentication  - path to authentication file" << std::endl;
    std::cout << "     -o   outdir          - folder where messages will be stored\e[0m" << std::endl;

    exit(SUCCESS_EXIT);
}

Arguments::Arguments()
{}

int Arguments::parse(int argc, char * argv[])
{
    if (argc == 1)
      printHelp();

    if (argc > 1)
      if (std::string(argv[1]).compare("-h") == 0 || std::string(argv[1]).compare("--help") == 0)
        printHelp();
      else
        setServer(argv[1]);
  
  int c;

  opterr = 0;

  for ( int i = 2; i < argc; i++)
    if ( std::string(argv[i]).substr(0,1) != "-" )
      if (std::string(argv[i-1]).compare("-p") == 0 || 
          std::string(argv[i-1]).compare("-c") == 0 ||
          std::string(argv[i-1]).compare("-C") == 0 ||
          std::string(argv[i-1]).compare("-a") == 0 ||
          std::string(argv[i-1]).compare("-o") == 0 
        // -p -c -C -a -o
        )
        continue;
      else {
        std::cerr << "ERROR: Unknown argument '" << argv[i] << "'." << std::endl;
        exit(ARGUMENT_ERROR);
      }

  while ((c = getopt(argc, argv, "p:TSc:C:dna:o:")) != EOF) {
    switch (c)
      {
      case 'p':
        setPort(optarg);
        break;
      case 'T':
        setTFlag();
        break;
      case 'S':
        setSFlag();
        break;
      case 'c':
        setCertFile(optarg);
        break;
      case 'C':
        setCertAddr(optarg);
        break;
      case 'd':
        setDFlag();
        break;
      case 'n':
        setNFlag();
        break;
      case 'a':
        setAuthFile(optarg);
        break;
      case 'o':
        setOutDir(optarg);
        break;
      case '?':
        if (optopt == 'p' || optopt == 'c' || optopt == 'C' || optopt == 'a' || optopt == 'o')
          std::cerr << "ERROR: Option '" << char(optopt) << "' requires an argument." << std::endl;
        else if (isprint (optopt))
          std::cerr << "ERROR: Unknown option '" << char(optopt) << "'." << std::endl;
        else
          std::cerr << "ERROR: Unknown option character" << std::endl;
        exit(ARGUMENT_ERROR);
      default:
        exit(ARGUMENT_ERROR);
        // abort();
      }
    }

 // TODO
 // checkArgumentsConsistence();
 // checkMandatoryArguments();
    return 0;
}

// ------------ SETS ------------

void Arguments::setServer(char* optarg) {
	server = std::string(optarg);
/* TODO
  if ( std::isdigit( server.substr(0,1) ) )
    ip4_flag = true;
*/
}

void Arguments::setTFlag() {
	if (t_flag == false) {
		t_flag = true;
//    std::cout << "TFlag nastaven" << std::endl;
  }
	else {
    std::cerr << "ERROR: TFlag" << std::endl;
    exit(FLAG_ALREADY_SET);
  }
}

void Arguments::setSFlag() {
  if (s_flag == false) {
    s_flag = true;
//    std::cout << "SFlag nastaven" << std::endl;
  }
  else {
    std::cerr << "ERROR: SFlag" << std::endl;
    exit(FLAG_ALREADY_SET);
  }
}

void Arguments::setDFlag() {
  if (d_flag == false) {
    d_flag = true;
//    std::cout << "DFlag nastaven" << std::endl;
  }
  else {
    std::cerr << "ERROR: DFlag" << std::endl;
    exit(FLAG_ALREADY_SET);
  }
}

void Arguments::setNFlag() {
  if (n_flag == false) {
    n_flag = true;
//    std::cout << "NFlag nastaven" << std::endl;
  }
  else {
    std::cerr << "ERROR: NFlag" << std::endl;
    exit(FLAG_ALREADY_SET);
  }
}

void Arguments::setPort(char* oprarg) {
  if (p_flag == false) {

    p_flag = true;

    char* pErr = NULL;
    long int l;

    l = strtol(optarg, &pErr, 10);

    if (*pErr != '\0') {
      std::cerr << "ERROR: Port number" << std::endl;
      exit(INCOMPATIBLE_PORT);
    }

    if (l <= 0 || l > 65535) {
      std::cerr << "ERROR: Port number" << std::endl;
      exit(INCOMPATIBLE_PORT);
    } 

    port = l;
//    std::cout << "Port nastaven" << std::endl;
  }
  else {
    std::cerr << "ERROR: PFlag" << std::endl;
    exit(FLAG_ALREADY_SET);
  }
}

void Arguments::setCertFile(char* optarg) {
  if (c_flag == false) {
    c_flag = true;
    cert_file = optarg;
  }
  else {
    std::cerr << "ERROR: cFlag" << std::endl;
    exit(FLAG_ALREADY_SET);
  }
}

void Arguments::setCertAddr(char* optarg) {
  if (C_flag == false) {
    C_flag = true;
    cert_addr = optarg;
  } 
  else {
    std::cerr << "ERROR: CFlag" << std::endl;
    exit(FLAG_ALREADY_SET);
  }
}

void Arguments::setOutDir(char* optarg) {
  if (o_flag == false) {
    o_flag = true;
    out_dir = optarg;
  }
  else {
    std::cerr << "ERROR: OFlag" << std::endl;
    exit(FLAG_ALREADY_SET);    
  }
}

// ------------ GETS ------------

std::string Arguments::getServer() {
  if (server.empty()) {
    std::cerr << "ERROR: Server not set!" << std::endl;
    exit(REQUIRED_ARGUMENT);
  }
  return server;
}

int Arguments::getPort() {
	return port;
}

std::string Arguments::getAuthFile() {
  if (auth_file.empty()) {
    std::cerr << "ERROR: Auth File not set!" << std::endl;
    exit(REQUIRED_ARGUMENT);
  }
  return auth_file;
}

std::string Arguments::getOutDir() {
  if (out_dir.empty()) {
    std::cerr << "ERROR: Out Dir not set!" << std::endl;
    exit(REQUIRED_ARGUMENT);
  }
	return out_dir;
}

std::string Arguments::getCertFile() {
  if (cert_file.empty()) {
    std::cerr << "ERROR: Certificate not set!" << std::endl;
    exit(REQUIRED_ARGUMENT);
  }
	return cert_file;
}

std::string Arguments::getCertAddr() {
  if (cert_addr.empty()) {
    std::cerr << "ERROR: Certificate folder not set!" << std::endl;
    exit(REQUIRED_ARGUMENT);
  }
	return cert_addr;
}

void Arguments::setAuthFile(char* optarg) {
  if (a_flag == false) {
    a_flag = true;
    auth_file = optarg;
  }
  else {
    std::cerr << "ERROR: AFlag" << std::endl;
    exit(FLAG_ALREADY_SET);
  }
}

std::string Arguments::getUsername() {
  if (a_flag == true)
    return username;
}
std::string Arguments::getPwd() {
  if (a_flag == true)
    return pwd;
}
