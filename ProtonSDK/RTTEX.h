#ifndef PROTONSDK__RTTEX__H
#define PROTONSDK__RTTEX__H
#include <cstdint>

#define C_RTFILE_PACKAGE_LATEST_VERSION 0
#define C_RTFILE_PACKAGE_HEADER "RTPACK"
#define C_RTFILE_PACKAGE_HEADER_BYTE_SIZE 6

struct RTFileHeader 
{
	char FileTypeID[C_RTFILE_PACKAGE_HEADER_BYTE_SIZE];
	uint8_t Version;
	uint8_t Reserved[1];
};

enum eCompressionType 
{
	C_COMPRESSION_NONE = 0,
	C_COMPRESSION_ZLIB = 1
};

struct RTPackHeader 
{
	RTFileHeader FileHeader;
	uint32_t CompressedSize;
	uint32_t DecompressedSize;
	uint8_t CompressionType;
	uint8_t Reserved[15];
};

#define RT_FORMAT_EMBEDDED_FILE 20000000
#define C_RTFILE_TEXTURE_HEADER "RTTXTR"
struct RTTexHeader 
{
	RTFileHeader FileHeader;
	int32_t Height;
	int32_t Width;
	int32_t Format;
	int32_t OriginalHeight;
	int32_t OriginalWidth;
	unsigned char bUsesAlpha;
	unsigned char bAlreadyCompressed;
	unsigned char ReservedFlags[2];
	int32_t MipmapCount;
	int32_t Reserved[16];
};

struct RTTexMipHeader 
{
	int32_t Height;
	int32_t Width;
	int32_t DataSize;
	int32_t MipLevel;
	int32_t Reserved[2];
};

#endif // PROTONSDK__RTTEX__H