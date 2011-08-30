// thanks to: http://www.netrino.com/Embedded-Systems/How-To/Big-Endian-Little-Endian
#ifndef ENDIANNESSH
#define ENDIANNESSH

	// for mac
	#undef BIG_ENDIAN
	//#define LITTLE_ENDIAN 1 // intell
	 
	#if defined(BIG_ENDIAN) && !defined(LITTLE_ENDIAN)

	  #define htonsex(A) (A)
	  #define htonlex(A) (A)
	  #define ntohsex(A) (A)
	  #define ntohlex(A) (A)

	#elif defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)

	  #define htonsex(A) ((((uint16_t)(A) & 0xff00) >> 8) | \
						(((uint16_t)(A) & 0x00ff) << 8))
	  #define htonlex(A) ((((uint32_t)(A) & 0xff000000) >> 24) | \
						(((uint32_t)(A) & 0x00ff0000) >> 8)  | \
						(((uint32_t)(A) & 0x0000ff00) << 8)  | \
						(((uint32_t)(A) & 0x000000ff) << 24))

	  #define ntohsex  htonsex
	  #define ntohlex  htonlex

	#else

	  #error "Must define one of BIG_ENDIAN or LITTLE_ENDIAN"

	#endif
#endif