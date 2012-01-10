#include "Utils.h"

#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <Poco/Glob.h>
#include <Poco/DigestStream.h>
#include <Poco/MD5Engine.h>
#include <iostream>

using Poco::File;
using Poco::Exception;
using Poco::Glob;
using Poco::trim;
using Poco::trimInPlace;
using Poco::DigestOutputStream;
using Poco::DigestEngine;
using Poco::MD5Engine;
using Poco::toLowerInPlace;
using Poco::toUpperInPlace;

using std::cout;
using std::endl;

void trim(string& str) {
	trimInPlace(str);
}

// create a directory if it doesnt exist yet.
bool createDirectory(string dir, bool relativeToDataDir, bool createSubDirectories) {
	try {
		trim(dir);
		if(relativeToDataDir) {
			dir = getDataPath() +dir;
		}
		Poco::File f(dir);
		try {
			if(f.isDirectory()) { // throws!
				return true;
			}
		}
		catch(...) {
			if(createSubDirectories) {
				f.createDirectories();
			}
			else {
				f.createDirectory();
			}
		}
	}
	catch(Poco::FileException& ex) {
		printf("Cannot create directory: '%s' reason:'%s'\n"
				,dir.c_str()
				,ex.displayText().c_str()
		);
		return false;
	}
	return true;
}

// tiny helper to check if a file exists.
bool fileExists(string path, bool relativeToDataDir) {
	bool ex = false;	
	try {
		if(relativeToDataDir) {
			path = getDataPath() +path;
		}
		Poco::File f(path);
		ex = f.exists();
	}
	catch(Poco::FileException& ex) {
		printf("Cannot create directory: '%s' reason:'%s'\n"
				,path.c_str()
				,ex.displayText().c_str()
		);
		return false;
	}
	return ex;
}

// Get Files by a globber filePathWithGlob example: /data/dir/filename* 
set<string> getFiles(string filePathWithGlob, bool relativeToDataDir) {
	if(relativeToDataDir) {
		filePathWithGlob = getDataPath() +filePathWithGlob;
	}
	set<string> files;
	try {
		Glob::glob(filePathWithGlob, files);
	}
	catch(...) {
		printf("Error while globbing...\n");
	}
	return files;
}

// string to md5
string md5(string input) {
	string hash;
	MD5Engine md5_engine;
	DigestOutputStream ostr(md5_engine);
	ostr << input;
	ostr.flush();
	const DigestEngine::Digest& digest = md5_engine.digest();
	hash = DigestEngine::digestToHex(digest);
	return hash;
}


string getDataPath() {
	string cwd = getCurrentWorkingDirectory();
	#if POCO_OS == POCO_OS_MAC_OS_X
		cwd += "../../../data/";		
	#else
		#error "Implement data directory for Windows"
	#endif
	
	return cwd;
}

string getCurrentWorkingDirectory() {
	Poco::Path p;
	return p.current();
}

void toLower(string& data) {
	toLowerInPlace(data);
}

void toUpper(string& data) {
	toUpperInPlace(data);
}



