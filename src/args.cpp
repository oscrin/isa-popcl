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

Arguments::Arguments() {}

int Arguments::parse(int argc, char * argv[])
{
    if (argc == 1)
      return -1;

    if (argc > 1) {
      if (std::string(argv[1]).compare("-h") == 0 || std::string(argv[1]).compare("--help") == 0)
        return -1;
    }
  
  int c;

  opterr = 0;

  for ( int i = 1; i < argc; i++)
    if ( std::string(argv[i]).substr(0,1) != "-" ) {
      if (std::string(argv[i-1]).compare("-p") == 0 || 
          std::string(argv[i-1]).compare("-c") == 0 ||
          std::string(argv[i-1]).compare("-C") == 0 ||
          std::string(argv[i-1]).compare("-a") == 0 ||
          std::string(argv[i-1]).compare("-o") == 0 
        // -p -c -C -a -o
        )
        continue;
      else {
        if (!server_flag) {
          setServer(argv[i]);
        } else {
          std::cerr << "ERROR: Unknown argument '" << argv[i] << "'." << std::endl;
          exit(UNKNOWN_ARGUMENT);
        }
      }
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
        setCAfile(optarg);
        break;
      case 'C':
        setCApath(optarg);
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
      }
    }

    assignPort();
    
    if (!checkArgumentsConsistence())
      return -2;
    if (!checkMandatoryArguments())
      return -3;

    return 0;
}

// ------------ CHECKS ------------

bool Arguments::checkArgumentsConsistence() {
  if (c_flag == true || C_flag == true) {

    if (t_flag == false && s_flag == false) {
      // ERROR ARGUMENT INCONSITENCE
      std::cout << "ERROR: Arguments inconsistency." << std::endl;
      return false;
    }
    return true;
  }
  return true;
}

bool Arguments::checkMandatoryArguments() {
  if (a_flag == false) {
    // ERROR - auth file must be set
    std::cout << "ERROR: Authfile not set." << std::endl;
    return false;
  }

  if (o_flag == false) {
    // ERROR - output dir must be set
    std::cout << "ERROR: Outdir not set." << std::endl;
    return false;
  }

  if (server_flag == false) {
    std::cout << "ERROR: Server not set." << std::endl;
    return false;
  }
  return true;
}

int Arguments::assignPort() {
    if (p_flag == false && t_flag == true) {
      port = 995;
      return port;
    }
    if (p_flag == false && s_flag == true) {
      port = 110;
      return port;
    }
    if (p_flag == false)
      port = 110;

    return port;
}

// ------------ SETS ------------

void Arguments::setServer(char* optarg) {
  server_flag = true;
	server = std::string(optarg);
}

void Arguments::setAuthFile(char* optarg) {
  if (a_flag == false) {
    a_flag = true;
    auth_file = optarg;
  }
  else {
    std::cerr << "ERROR: Argument '-a' set more than once." << std::endl;
    exit(DOUBLE_ARGUMENT);
  }
}

void Arguments::setTFlag() {
	if (t_flag == false) {
		t_flag = true;
//    std::cout << "TFlag nastaven" << std::endl;
  }
	else {
    std::cerr << "ERROR: Argument '-T' set more than once." << std::endl;
    exit(DOUBLE_ARGUMENT);
  }
}

void Arguments::setSFlag() {
  if (s_flag == false) {
    s_flag = true;
//    std::cout << "SFlag nastaven" << std::endl;
  }
  else {
    std::cerr << "ERROR: Argument '-S' set more than once." << std::endl;
    exit(DOUBLE_ARGUMENT);
  }
}

void Arguments::setDFlag() {
  if (d_flag == false) {
    d_flag = true;
//    std::cout << "DFlag nastaven" << std::endl;
  }
  else {
    std::cerr << "ERROR: Argument '-d' set more than once." << std::endl;
    exit(DOUBLE_ARGUMENT);
  }
}

void Arguments::setNFlag() {
  if (n_flag == false) {
    n_flag = true;
//    std::cout << "NFlag nastaven" << std::endl;
  }
  else {
    std::cerr << "ERROR: Argument '-n' set more than once." << std::endl;
    exit(DOUBLE_ARGUMENT);
  }
}

void Arguments::setPort(char* oprarg) {
  if (p_flag == false) {

    p_flag = true;

    char* pErr = NULL;
    long int l;

    l = strtol(optarg, &pErr, 10);

    if (*pErr != '\0') {
      std::cerr << "ERROR: Incompatible port number." << std::endl;
      exit(INCOMPATIBLE_PORT);
    }

    if (l <= 0 || l > 65535) {
      std::cerr << "ERROR: Incompatible port number." << std::endl;
      exit(INCOMPATIBLE_PORT);
    } 

    port = l;
  }
  else {
    std::cerr << "ERROR: Argument '-p' set more than once." << std::endl;
    exit(DOUBLE_ARGUMENT);
  }
}

void Arguments::setCAfile(char* optarg) {
  if (c_flag == false) {
    c_flag = true;
    cert_file = optarg;
  }
  else {
    std::cerr << "ERROR: Argument '-c' set more than once." << std::endl;
    exit(DOUBLE_ARGUMENT);
  }
}

void Arguments::setCApath(char* optarg) {
  if (C_flag == false) {
    C_flag = true;
    cert_addr = optarg;
  } 
  else {
  std::cerr << "ERROR: Argument '-C' set more than once." << std::endl;
    exit(DOUBLE_ARGUMENT);
  }
}

void Arguments::setOutDir(char* optarg) {
  if (o_flag == false) {
    o_flag = true;
    out_dir = optarg;
  }
  else {
  std::cerr << "ERROR: Argument '-o' set more than once." << std::endl;
    exit(DOUBLE_ARGUMENT);    
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
    std::cerr << "ERROR: Authfile not set!" << std::endl;
    exit(REQUIRED_ARGUMENT);
  }
  return auth_file;
}

std::string Arguments::getOutDir() {
  if (out_dir.empty()) {
    std::cerr << "ERROR: Outdir not set!" << std::endl;
    exit(REQUIRED_ARGUMENT);
  }
	return out_dir;
}

std::string Arguments::getCAfile() {
  if (cert_file.empty()) {
    std::cerr << "ERROR: Certificate not set!" << std::endl;
    exit(REQUIRED_ARGUMENT);
  }
	return cert_file;
}

std::string Arguments::getCApath() {
  if (cert_addr.empty()) {
    std::cerr << "ERROR: Certificate folder not set!" << std::endl;
    exit(REQUIRED_ARGUMENT);
  }
	return cert_addr;
}

bool Arguments::getTFlag(){
  return t_flag;
}
bool Arguments::getSFlag() {
  return s_flag;
}

bool Arguments::getNFlag() {
  return n_flag;
}

bool Arguments::getDFlag() {
  return d_flag;
}
