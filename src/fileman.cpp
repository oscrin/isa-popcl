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

std::string FileManager::generateEmailFileByMID(std::string mid) {

	messageID = mid;

	std::ofstream outfile(out_dir + "/" + messageID);
	outfile.close();

	return messageID;
}

std::string FileManager::generateEmailFileByUIDL(std::string uidl) {

	messageUIDL = uidl;

	std::ofstream outfile(out_dir + "/" + messageUIDL);
	outfile.close();

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

	std::string tsvFile = out_dir + "/.mail.tsv";

	if (searchTsvFile(uidl.c_str(), mid.c_str()) == true)
		return false;

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

bool FileManager::searchTsvFile(const char * uidl, const char * mid) {

std::string tsvFile = out_dir + "/.mail.tsv";

	std::ifstream aF;
    std::string nl;
    std::string u;
    std::string m;
    int sep;

    aF.open(tsvFile);
    if (aF.is_open()) {
    	while (!aF.eof()) {
        	getline(aF,nl);

        	if ((sep = nl.find("\t")) != int(std::string::npos)) {
            	u = nl.substr(0,sep);
        		m = nl.substr(sep,nl.length()-sep);

	        	if (uidl != NULL) {
	        		if (u.compare(uidl) == 0)
	        			return true;
	        	}
	        	if (mid != NULL) {
	        		if (m.compare(mid) == 0)
	        			return true;
	        	}	
        	}
   		}

   		return false;
    }

    return false;
}