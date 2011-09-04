#ifndef DICTIONARYMAPH
#define DICTIONARYMAPH

#include <map>
#include <string>

using std::map;
using std::string;


class Dictionary;
struct DictionaryMap {
	DictionaryMap(DictionaryMap& other);
	DictionaryMap(); 
	
	string type_name;
	map<string, Dictionary> children;
	bool is_array;
	
};

#endif