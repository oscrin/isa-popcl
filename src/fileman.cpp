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
#include <unistd.h>

bool FileManager::createOutDir( std::string dir) {
    int status;

    out_dir = dir;

    status = access(out_dir.c_str(), W_OK);
    if (status == 0)
    {
        return true;
    }
    else {
	    
	    status = mkdir(out_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	    if (status == -1) {
	      std::cerr << "ERROR: Directory cannot be created." << std::endl;
	      exit(DIR_NOT_CREATED); 
	    }

	    return true;
	}
}

std::string FileManager::generateEmailNameByMID(std::string content) {
	//TODO

	// std::cout << "GENERATE1:\n" << content << std::endl;

	// Message-Id: <20171107180800.3AFCA6CC@centrum.cz>

	int index = (content.find("Message-Id:") == std::string::npos) ? content.find("Message-ID:") : content.find("Message-Id:");

	if (index == int(std::string::npos)) {
		//TODO
		std::cout << "Message-Id not found." << std::endl;
		return "tmp";
	}
	content.erase(0, index);
	content.erase(0,content.find("<")+1);
	messageID = content.substr(0, content.find(">"));

//	std::cout << "ID:\n" << messageID << std::endl;
//	std::string id = content.substr(i, 
	return messageID;
}

std::string FileManager::generateEmailNameByUIDL(std::string uidl) {

	uidl.erase(0,4);
	uidl.erase(0,uidl.find(" ")+1);
	uidl = uidl.substr(0,uidl.length()-2);
//	std::cout << "UIDL: '" << uidl << "'" << std::endl;

	messageUIDL = uidl;

	return messageUIDL;
}

bool FileManager::saveEmailFile(std::string emailFile, std::string content) {

	if (!out_dir.empty()) {

		emailFile = out_dir + "/" + emailFile;

		std::ofstream outfile(emailFile);
		outfile << content;
		outfile.close();
		return true;

	} else
		return false;
}

bool FileManager::actualizeTsvFile(std::string uidl, std::string mid) {

	std::string tsvFile = out_dir + "/.mail";

	std::ifstream aF;
    std::string nl;

    aF.open(tsvFile);
    if (aF.is_open()) {
    	while (!aF.eof()) {
        	getline(aF,nl);
   		}
    }

	if (access( tsvFile.c_str(), F_OK ) != -1) { // file exists
	
		std::ofstream output(tsvFile, std::ios::app);
		output << uidl << "\t" << mid << "\n";
	
	}
	else {

		std::ofstream output(tsvFile);
		output << uidl << "\t" << mid << "\n";

	}
	return true;
}