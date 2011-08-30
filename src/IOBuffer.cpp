#include "IOBuffer.h"
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

bool IOBuffer::ignore(uint32_t numBytes) {
	consumed += numBytes;
	recycle();	
	return true;
}
bool IOBuffer::reuse(uint32_t numBytes) {
	consumed -= numBytes;
	return true;
}

void IOBuffer::recycle() {
	if(consumed != published) {
		return;
	}
	consumed = 0;
	published = 0;
}

bool IOBuffer::consumeLine(string& line) {
	for(int i = 0; i < published; ++i) {
		if(buffer[i] == 0x00) {
			printf("line end on %d\n", i);
		}
		else {
			printf("c: %d\n", buffer[i]);
		}
	}
	
	return false;
}

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

int IOBuffer::consumeUntil(string until, string& found) {
	int search_size = until.size();
	const char* search = until.c_str();
	int k = 0;
	for(int i = consumed; i < published; ++i) {
		cout << "store:" << (char)buffer[i] << endl;
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
				cout << "+++++++++++++++++++++++++++ FOUND UNTIL: "<< until << " ++++++++++++++++++++" << endl;
				consumed += (i+search_size);
				return consumed;
			}
		}
	}
	found = "";
	return 0;
}	

void IOBuffer::consumeByte(uint8_t& byte) {
	byte = buffer[consumed];
	consumed++;
}

void IOBuffer::consumeBytes(uint8_t* buf, uint32_t numBytes) {
	memcpy(buf,buffer+consumed, numBytes);
	consumed += numBytes;
}

void IOBuffer::printHex(uint32_t start, uint32_t end) {
	if(start == 0 && end == 0) {
		start = 0;
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

void IOBuffer::consumeString(string& str, uint32_t numBytes) {
	str.assign((char*)buffer+consumed, numBytes);
	consumed += numBytes;
}

void IOBuffer::consumeUint8(uint8_t& buff) {
	consumeByte(buff);
}

void IOBuffer::consumeUint16(uint16_t& buff) {
	memcpy(&buff, buffer+consumed, 2);
	consumed += 2;
}

void IOBuffer::consumeUint32(uint32_t& buff) {
	memcpy(&buff, buffer+consumed, 4);
	consumed += 4;
}