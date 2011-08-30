#ifndef IOBUFFERH
#define IOBUFFERH

#include <inttypes.h>
#include <string>
#include <algorithm>

using std::string;

// Based on CRTMP server source code.


#define GET_IB_POINTER(x) 				 ((uint8_t *)((x).buffer + (x).consumed))
#define GET_AVAILABLE_BYTES_COUNT(x)     ((x).published - (x).consumed)

class IOBuffer {
public:	
	uint8_t* 	buffer;
	uint32_t	size;
	uint32_t	published;
	uint32_t	consumed;
	uint32_t	min_chunk_size;
	
public:
	IOBuffer();
	virtual ~IOBuffer();
	void setup();
	void setup(uint32_t expectedSize);
	bool ensureSize(uint32_t expectedSize);
	void cleanup();
	bool moveData();
	void setMinChunkSize(uint32_t minSize);
	uint32_t getMinChunkSize();
	uint8_t* getPtr();
	
	bool reuse(uint32_t numBytes); // opposite of ignore
	bool ignore(uint32_t numBytes); 
	void recycle();
	
	bool storeBytes(const char* someData, const uint32_t numBytes);
	bool storeBytes(const uint8_t* someData, const uint32_t numBytes);
	void storeByte(uint8_t byte);
	void storeRepeat(uint8_t byte, uint32_t numBytes);
	
	bool consumeLine(string& line);
	int consumeUntil(uint8_t until, string& found);
	int consumeUntil(string until, string& found);
	void consumeByte(uint8_t& byte);
	void consumeBytes(uint8_t* buff, uint32_t numBytes);

	void consumeUint8(uint8_t &buff);
	void consumeUint16(uint16_t& buff);
	void consumeUint32(uint32_t& buff);
	void consumeString(string& str, uint32_t size);
	inline uint8_t getAt(uint8_t position);
	
	void printHex(uint32_t start = 0, uint32_t end = 0);
	
};


inline uint8_t IOBuffer::getAt(uint8_t position) {
	return buffer[position];
}
#endif