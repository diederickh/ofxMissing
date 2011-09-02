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

	
	// setup
	void setup();
	void setup(uint32_t expectedSize);
		
	// moving the read head
	bool reuse(uint32_t numBytes); // opposite of ignore
	bool ignore(uint32_t numBytes); 
	
	// store data
	bool storeBytes(const char* someData, const uint32_t numBytes);
	bool storeBytes(const uint8_t* someData, const uint32_t numBytes);
	void storeByte(uint8_t byte);
	void storeUInt8(uint8_t byte);
	void storeUInt16(uint16_t data);
	void storeUInt32(uint32_t data);
	void storeRepeat(uint8_t byte, uint32_t numBytes); // rename!
	void storeBuffer(IOBuffer& other); // copies only stored data
	void storeBuffer(IOBuffer& other, uint32_t numBytes);

	void storeString(string& data);
	void storeBigEndianUInt16(uint16_t data);
	void storeBigEndianUInt32(uint32_t data);
	
	// get the number of bytes published.
	uint32_t getNumBytesStored();
	
					
	// reading data back from buffer moving read head
	int consumeUntil(uint8_t until, string& found);
	int consumeUntil(string until, string& found);
	void consumeBytes(uint8_t* buff, uint32_t numBytes);

	string consumeString(uint32_t upToNumBytes);

	uint8_t consumeByte();
	uint8_t  consumeUInt8();
	uint16_t consumeUInt16();
	uint32_t consumeUInt32();
	uint64_t consumeUInt64();

	int8_t  consumeInt8();
	int16_t consumeInt16();
	int32_t consumeInt32();
	int64_t consumeInt64();
	
	uint16_t consumeLittleEndianUInt16();
	uint32_t consumeLittleEndianUInt32();
	uint64_t consumeLittleEndianUInt64();
	
	
	// helpers
	inline uint8_t getAt(uint8_t position);
	void printHex(uint32_t start = 0, uint32_t end = 0);
	void recycle();
	bool ensureSize(uint32_t expectedSize);
	void cleanup();
	bool moveData();
	void setMinChunkSize(uint32_t minSize);
	uint32_t getMinChunkSize();
	uint8_t* getPtr();
};


inline uint8_t IOBuffer::getAt(uint8_t position) {
	return buffer[position];
}
#endif