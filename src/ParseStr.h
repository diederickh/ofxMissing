#ifndef PARSESTRH
#define PARSESTRH

/**
 * While working on simple socket applications it's sometimes easy
 * to use a url like string to pass variables. This class parses these
 * strings.
 * 
 * Example:
 * name=Somename&email=info@roxlu.com&number=10&other=whatever
 *
 * Bote that we only handle simple strings like these, no other fancy stuff
 * which is often not necessary. It tries to mimic "parse_str" of PHP. 
 *
 * Example usage:
 * ==============
 *	string test = "name=Diederick Huijbers&qrcode=123123123&question=1&answer=10&url=http://www.roxlu.com";
 *	ParseStrResult res = parse_str(test);
 *	try {
 *		cout << "name: "  << res["name"] << endl;
 *		cout << "qr: "  << res["qrcode"] << endl;
 *		cout << "url: "  << res["url"] << endl;
 *		
 *		cout << res["somekey"] << endl;
 *	}
 *	catch(string msg) {
 *		cout << msg << endl;
 *	}
 *
 */

#include <map>
#include <iostream>
#include <string>
#include "Poco/Stringtokenizer.h"

using std::string;
using std::cout;
using std::endl;
using Poco::StringTokenizer;
using std::map;
 
// result container for the parsed string.
class ParseStrResult {
public:
	ParseStrResult() {
	}
	bool hasField(string key);
	string operator[](const string& key);
	map<string, string> vars;
	
};

ParseStrResult parse_str(string str);

inline string ParseStrResult::operator[](const string& key) {
	if(vars.find(key) == vars.end()) {
		throw "ParseStrResult key: '" +key +"' not found";
	}
	return vars[key];
}

#endif