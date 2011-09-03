#ifndef DICTIONARYH
#define DICTIONARYH

// thanks: http://wiki.rtmpd.com/myfirstapp/variant?s[]=amf

#include <inttypes.h>
#include <string>
#include <sstream>
#include <map>
#include <iomanip>

#define IN_MAP(m,k)	((bool)((m).find((k))!=(m).end()))
#define VAR_INDEX_VALUE "__index__value__"

using namespace std;

enum DictionaryType {
	 D_NULL		=	0
	,D_BOOL		= 	1
	
	,D_INT8		=	2
	,D_INT16	=	3
	,D_INT32	=	4
	,D_INT64	=	5
	
	,D_UINT8	=	6
	,D_UINT16	=	7
	,D_UINT32	=	8
	,D_UINT64	=	9
	
	,D_DOUBLE	= 	10
	,D_STRING	=	11
	,D_MAP		= 	12
};

struct DictionaryMap;

class Dictionary {
public:
	// constructors.
	Dictionary();
	Dictionary(const Dictionary& val);
	Dictionary(const bool& val);
	Dictionary(const double &val);
	Dictionary(const string &val);
	
	Dictionary(const int8_t &val);
	Dictionary(const int16_t &val);
	Dictionary(const int32_t &val);
	Dictionary(const int64_t &val);
	Dictionary(const uint8_t &val);
	Dictionary(const uint16_t &val);
	Dictionary(const uint32_t &val);
	Dictionary(const uint64_t &val);
	
	// assignment operators.
	Dictionary& operator=(const Dictionary& val);
	Dictionary& operator=(const bool &val);
	Dictionary& operator=(const double &val);
	Dictionary& operator=(const string &val);
	Dictionary& operator=(const char* chars);
	
	Dictionary& operator=(const int8_t &val);
	Dictionary& operator=(const int16_t &val);
	Dictionary& operator=(const int32_t &val);
	Dictionary& operator=(const int64_t &val);
	
	Dictionary& operator=(const uint8_t &val);
	Dictionary& operator=(const uint16_t &val);
	Dictionary& operator=(const uint32_t &val);
	Dictionary& operator=(const uint64_t &val);
	
	Dictionary& operator[](const string& key);
	Dictionary& operator[](const char* key);
	Dictionary& operator[](const uint32_t& key);
	
	// retrieve as a forced type
	uint8_t  getAsUInt8();
	uint16_t getAsUInt16();
	uint32_t getAsUInt32();
	uint64_t getAsUint64();
	
	int8_t 	 getAsInt8();
	int16_t  getAsInt16();
	int32_t  getAsInt32();
	int64_t  getAsInt64();
	
	// Change internal type
	Dictionary& toInt8();
	Dictionary& toInt16();
	Dictionary& toInt32();
	Dictionary& toInt64();
	Dictionary& toUInt8();
	Dictionary& toUInt16();
	Dictionary& toUInt32();
	Dictionary& toUint64();
	
	// type casting
	operator bool();
	operator int8_t();
	operator int16_t();
	operator int32_t();
	operator int64_t();
	operator uint8_t();
	operator uint16_t();
	operator uint32_t();
	operator uint64_t();
	operator double();
	operator string();
	
	bool serializeToJSON(string &result);
	void escapeJSON(string &value);
	
	// string functions
	void replaceString(string& target, string search, string replacement);
	
	// destructor related
	void reset();
	
	// array related
	bool isArray();
	void isArray(bool makeArray);
	uint32_t getMapSize();
	uint32_t getMapDenseSize();
	void pushToArray(Dictionary dict);

	void removeKey(const string& key);
	void removeAt(const uint32_t index);

	DictionaryType type;
	map<string, Dictionary>::iterator begin();
	map<string, Dictionary>::iterator end();
private:	
	
	union {
		bool 		b;
		int8_t 		i8;
		int16_t		i16;
		int32_t		i32;
		int64_t		i64;
		uint8_t		ui8;
		uint16_t	ui16;
		uint32_t	ui32;
		uint64_t	ui64;
		double		d;
		string*		s;
		DictionaryMap* m;
	} value;
	
	void copyFrom(const Dictionary& other);
};
#endif