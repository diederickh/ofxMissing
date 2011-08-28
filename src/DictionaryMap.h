#ifndef DICTIONARYMAPH
#define DICTIONARYMAPH

#include <map>
#include <string>
#include "Dictionary.h"

using std::map;
struct DictionaryMap {
	string type_name;
	map<string, Dictionary> children;
	bool is_array;
	
	DictionaryMap(DictionaryMap& other) {
		type_name = other.type_name;
		children = other.children;
		is_array = other.is_array;
	}	
	
	DictionaryMap() {
		is_array = false;
	}
};

#endif