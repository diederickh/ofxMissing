#include "DictionaryMap.h"
#include "Dictionary.h"

DictionaryMap::DictionaryMap() {
	is_array = false;
}

DictionaryMap::DictionaryMap(DictionaryMap& other) {
	type_name = other.type_name;
	children = other.children;
	is_array = other.is_array;
}