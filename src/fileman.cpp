/* Faculty of Information Technology, Brno University of Technology
*  Subject: ISA (Network Applications and Network Administration) - Project 1
*  Description: POP3 Client
*  Date created: October 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: fileman.cpp - FileManager class implementation
*/

#include "fileman.h"
#include "error.h"

#include <sys/stat.h>
#include <iostream>
#include <fstream>

bool FileManager::createOutDir( std::string dir) {
    int status;

    out_dir = dir;

    status = mkdir(out_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (status == -1) {
      std::cerr << "ERROR: Directory cannot be created." << std::endl;
      exit(DIR_NOT_CREATED); 
    }

    return true;
}

std::string FileManager::generateEmailFileName() {
	//TODO
	return "001";
}

bool FileManager::saveEmailFile(std::string emailFile, std::string content) {

if (!out_dir.empty()) {

	emailFile = out_dir + "/" + emailFile;

	std::ofstream outfile (emailFile);
	outfile << content;
	outfile.close();
	return true;

} else
	return false;
}