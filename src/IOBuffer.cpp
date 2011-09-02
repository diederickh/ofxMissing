#include "IOBuffer.h"
#include "Endianness.h"

#include "ofMain.h" // for debuggin
IOBuffer::IOBuffer() 
:buffer(NULL)
,size(0)
,published(0)
,consumed(0)
,min_chunk_size(4096)
{
}

IOBuffer::~IOBuffer() {
	cleanup();
}

void IOBuffer::setup() {
	setup(min_chunk_size);	
}

void IOBuffer::setup(uint32_t expectedSize) {
	if( (buffer != NULL)
		|| (size != 0)
		|| (published != 0)
		|| (consumed != 0)
	)
	{
		printf("iobuffer error: buffer has been setup already\n");
		return;
	}
	
	ensureSize(expectedSize);
}

bool IOBuffer::ensureSize(uint32_t expectedSize) {
	
	// 1. we have enough space
	if(published + expectedSize <= size) {
		return true;
	}
	
	// 2. not enough space, try to move it.
	moveData();
	
	// 3. check if we have enough space after moving..
	if(published + expectedSize <= size) {
		return true;
	}
	
	// 4. not enough space. 
	// first allocate at least 1/3 of what we have and no less then min_chunk_size
	if( (published + expectedSize - size) < (size / 3)) {
		expectedSize = size + size / 3 - published;
	}
	if(expectedSize < min_chunk_size) {
		expectedSize = min_chunk_size;
	}
	
	// 5. create a new buffer.
	uint8_t* tmp_buffer = new uint8_t[published + expectedSize];
	
	// 6. copy exising data to tmp buffer
	if(buffer != NULL) {
		memcpy(tmp_buffer, buffer, published);
		delete[] buffer;
	}
	buffer = tmp_buffer;
	
	// 7. update our size.
	size = published + expectedSize;
	return true;
}

uint8_t* IOBuffer::getPtr() {
	return buffer;
}

bool IOBuffer::moveData() {
	if(published - consumed <= consumed) {
		memcpy(buffer, buffer+consumed, published - consumed);
		published = published - consumed;
		consumed = 0;
	}
	return true;
}

void IOBuffer::cleanup() {
	if(buffer != NULL) {
		delete[] buffer;
		buffer = NULL;
	}	
	size = 0;
	consumed = 0;
	published = 0;
}

void IOBuffer::setMinChunkSize(uint32_t minSize) {
	assert(minSize > 0 && minSize < 16 * 1024 * 1024);
	min_chunk_size = minSize;
}

uint32_t IOBuffer::getMinChunkSize() {
	return min_chunk_size;
}

void IOBuffer::storeByte(uint8_t byte) {
	ensureSize(1);
	buffer[published] = byte;
	published++;
}

bool IOBuffer::storeBytes(const uint8_t* someData, const uint32_t numBytes) {
	if(!ensureSize(numBytes)) {
		return false;
	}
	memcpy(buffer+published, someData, numBytes);
	published += numBytes;
	return true;
}

bool IOBuffer::storeBytes(const char* someData, const uint32_t numBytes) {
	if(!ensureSize(numBytes)) {
		return false;
	}
	memcpy(buffer+published, someData, numBytes);
	published += numBytes;
	return true;
}


void IOBuffer::storeRepeat(uint8_t byte, uint32_t numBytes) {
	ensureSize(numBytes);
	memset(buffer + published, byte, numBytes);
	published += numBytes;
}

void IOBuffer::storeUInt8(uint8_t byte) {
	storeByte(byte);
}

void IOBuffer::storeUInt16(uint16_t data) {
	ensureSize(16);
	memcpy(buffer+published, &data, 2);
	published += 2;
}

void IOBuffer::storeUInt32(uint32_t data) {
	ensureSize(32);
	memcpy(buffer+published, &data, 4);
	published += 4;
}

void IOBuffer::storeBigEndianUInt16(uint16_t data) {
	ensureSize(16);
	data = htonsex(data);
	memcpy(buffer+published, &data, 2);
	published += 2;
}

void IOBuffer::storeBigEndianUInt32(uint32_t data) {
	ensureSize(4);
	data = htonlex(data);
	memcpy(buffer+published, &data, 4);
	published += 4;
}

void IOBuffer::storeString(string& data) {
	uint32_t len = (uint32_t)data.length();
	ensureSize(len);
	memcpy(buffer+published, (uint8_t *)data.c_str(), len);
	published += len;
}

// copy data from another buffer.
void IOBuffer::storeBuffer(IOBuffer& other) {
	storeBuffer(other, other.getNumBytesStored());	
}

void IOBuffer::storeBuffer(IOBuffer& other, uint32_t numBytes) {
	ensureSize(numBytes);
	memcpy(buffer+published, other.buffer, numBytes);
	published += numBytes;
}


bool IOBuffer::ignore(uint32_t numBytes) {
	consumed += numBytes;
	recycle();	
	return true;
}
bool IOBuffer::reuse(uint32_t numBytes) {
	consumed -= numBytes;
	return true;
}

uint32_t IOBuffer::getNumBytesStored() {
	return published;
}

void IOBuffer::recycle() {
	if(consumed != published) {
		return;
	}
	consumed = 0;
	published = 0;
}


// Retrieve from buffer and return data
//------------------------------------------------------------------------------
void IOBuffer::consumeBytes(uint8_t* buf, uint32_t numBytes) {
	memcpy(buf,buffer+consumed, numBytes);
	consumed += numBytes;
}

uint8_t IOBuffer::consumeByte() {
	return consumeUInt8();
}

uint8_t IOBuffer::consumeUInt8() {
	uint8_t val = 0;
	memcpy(&val, buffer+consumed, 1);
	consumed += 1;
	return val;
}

uint16_t IOBuffer::consumeUInt16() {
	uint16_t val = 0;
	memcpy(&val, buffer+consumed, 2);
	consumed += 2;
	return val;

}

uint32_t IOBuffer::consumeUInt32() {
	uint32_t val = 0;
	memcpy(&val, buffer+consumed, 4);
	consumed += 4;
	return val;
}

uint64_t IOBuffer::consumeUInt64() {
	uint64_t val = 0;
	memcpy(&val, buffer+consumed, 8);
	consumed += 8;
	return val;
}

int8_t  IOBuffer::consumeInt8() {
	int8_t val = 0;
	memcpy(&val, buffer+consumed, 1);
	consumed += 1;
	return val;
}

int16_t IOBuffer::consumeInt16() {
	int16_t val = 0;
	memcpy(&val, buffer+consumed, 2);
	consumed += 2;
	return val;
}

int32_t IOBuffer::consumeInt32() {
	int32_t val = 0;
	memcpy(&val, buffer+consumed, 4);
	consumed += 4;
	return val;
}

int64_t IOBuffer::consumeInt64() {
	int64_t val = 0;
	memcpy(&val, buffer+consumed, 8);
	consumed += 8;
	return val;
}

// when you assume the data is big endian, convert it to little endian.
// -----------------------------------------------------------------------------
uint16_t IOBuffer::consumeLittleEndianUInt16() {
	uint16_t val = 0;
	memcpy(&val, buffer+consumed, 2);
	consumed += 2;
	val = ntohsex(val);
	return val;
}

uint32_t IOBuffer::consumeLittleEndianUInt32() {
	uint32_t val = 0;
	memcpy(&val, buffer+consumed, 4);
	consumed += 4;
	val = ntohlex(val);
	return val;
}

uint64_t IOBuffer::consumeLittleEndianUInt64() {
	uint64_t val = 0;
	printf("@todo IOBuffer, no handle for little endian 64 yet\n");
	/*
	memcpy(&val, buffer+consumed, 8);
	consumed += 8;
	val = ntohsex(val);
	*/
	return val;
}




// Searching for bytes in buffer and returning strings
// -----------------------------------------------------------------------------

// returns where we found the string, or 0 when not found.
int IOBuffer::consumeUntil(uint8_t until, string& found) {
	for(int i = consumed; i < published; ++i) {
		found.push_back((char)buffer[i]);
		if(buffer[i] == until) {
			consumed += i;
			return i;
		}
	}
	return 0;
}

// consume up to the given string, if not foudn return 0 else bytes consumed
int IOBuffer::consumeUntil(string until, string& found) {
	int search_size = until.size();
	const char* search = until.c_str();
	int k = 0;
	for(int i = consumed; i < published; ++i) {
		//cout << "store:" << (char)buffer[i] << endl;
		found.push_back((char)buffer[i]);
		if(buffer[i] == search[0]) {
			// out of buffer.
			if( (i + search_size) > published) {
				return 0;
			} 
			// check if next couple of chars is the search string.
			for(k = 0; k < search_size; ++k) {
				if(buffer[i+k] != search[k]) {
					break;
				}
			}
			// we found the search string, update consumed!
			if(k == search_size) {
				found.append((char*)buffer+(i+1), search_size-1);
				consumed += (i+search_size);
				return consumed;
			}
		}
	}
	found = "";
	return 0;
}	

string IOBuffer::consumeString(uint32_t upToNumBytes) {
	string str((char*)buffer+consumed, upToNumBytes);
	consumed += upToNumBytes;
	return str;
}


// helpers
//------------------------------------------------------------------------------
void IOBuffer::printHex(uint32_t start, uint32_t end) {
	if(start == 0 && end == 0) {
		start = consumed;
		end = published;
	}
	if(end > published) {
		end = published;
	}
	if(start > end) {
		start = end;		
	}
	int c = 0;
	for(int i = start,c=0; i < end; ++i,++c) {
		printf("%02X ", buffer[i]);
		if(c > 0 && c % 20 == 0) {
			printf("\n");
		}
	}
	printf("\n");
}

