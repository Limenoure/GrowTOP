#pragma once
#include <cstring>
#include <string>
#include <random>
#include <vector>
#include <string_view>
#include <ProtonSDK/RTTEX.h>
#include <cmath>
#include <cstdlib>

#define MAKE_RGB(r, g, b) (((uint32_t)(r) << 8) + ((uint32_t)(g) << 16) + ((uint32_t)(b) << 24))
#define MAKE_RGBA(r, g, b, a) (((uint32_t)(r) << 8) + ((uint32_t)(g) << 16) + ((uint32_t)(b) << 24) + ((uint32_t)(a)))
#define FROM_RGBA(r, g, b, a) ((uint32_t)(a) | (uint32_t)(r) << 8 | (uint32_t)(g) << 16 | (uint32_t)(b) << 24)

#define GET_BLUE(p)     ((p) >> 24)
#define GET_GREEN(p)    (((p) & 0x00FF0000) >> 16)
#define GET_RED(p)      (((p) & 0x0000FF00) >>  8)
#define GET_ALPHA(p)    ((p) & 0x000000FF)

constexpr uint64_t HashStringFNV(const std::string_view& input) 
{
    if (input.empty())
    {
        return 0;
    }

    uint64_t hash = 0;
    for (size_t i = 0; i < input.size(); i++) 
    {
        hash += input[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    hash ^= (hash >> 23);
    hash += (hash << 27);

    return hash;
}
constexpr uint64_t operator "" _FNV(const char* str, size_t len) 
{
    return HashStringFNV(std::string_view(str, len));
}

class Utils
{
public:
    static std::vector<std::string> StringTokenize(const std::string& text, const std::string& delim = "|");
    static std::vector<std::string> SplitString(const std::string& text, const std::string& delim);
    static bool SeparateString (const char str[], int num, char delim, char* pResult);
    static std::string SeparateStringSTL(std::string input, int index, char delim);
    static void StringReplace(const std::string& what, const std::string& with, std::string& in);
    static bool ContainsGTSwear(const std::string& text);
    static bool IsStringNumber(const std::string& str);
    static bool IsOnlyAlphabet(const std::string& str);

	static void RemoveBadCharacters(std::string& str);
	static void TrimSpacesLeftOfString(std::string& s);
	static void TrimSpacesRightOfString(std::string& s);
    static void TrimSpaceBothSidesOfString(std::string& s);
    static void RemoveExtraSpaces(std::string& str);
    static void RemoveSpaces(std::string& str);
    static void FFMFThisString(std::string& str);

    static uint32_t HashString(const void* pMem, uint32_t dataLength);
    static int32_t GenerateUniqueTag(const std::string& name, const std::string& relativeId, const std::string& key);
    static std::string ResolveIPAddress(const uint32_t& ip);
    static std::string NormalizeString(const std::string& str);

    static std::string HashPassword(const std::string& username, const std::string& password);
    static std::string CreateMD5(const std::string& input);

    static std::string GetLogonVariantString(float gVer);

    static bool SaveToFile(const std::string& fName, const std::string& content);
    static unsigned int GetHashOfFile(const std::string& fName);

    static std::string SHA256Crypt(std::string input);
    static std::string MD5Crypt(std::string input);
    static std::string GenerateRid(const std::string& game_version, const int& protocol, const std::string& rid);
};

class Randomizer 
{
public:
    template<typename T> static T Get(T min, T max) 
    {
        static std::mt19937 rng{ std::random_device()() };
        if constexpr (std::is_integral<T>::value) 
        {
            std::uniform_int_distribution<T> distrib(min, max);
            return distrib(rng);
        }
        else if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value) 
        {
            std::uniform_real_distribution distrib(min, max);
            return static_cast<T>(distrib(rng));
        }

        return 0;
    }

    template<typename T> static T Get() 
    {
        static std::mt19937 rng{ std::random_device()() };

        if constexpr (std::is_integral<T>::value) 
        {
            std::uniform_int_distribution<T> distrib;
            return distrib(rng);
        }
        else if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value) 
        {
            std::uniform_real_distribution distrib;
            return static_cast<T>(distrib(rng));
        }

        return 0;
    }
};

template <typename T> void MemorySerialize(T* pVal, uint8_t* pMem, int &offsetInOut, bool bWriteToMem) 
{
    if (pVal) 
    {
        if (bWriteToMem)
        {
            memcpy(&pMem[offsetInOut], pVal, sizeof(T));
        }
        else
        {
            memcpy(pVal, &pMem[offsetInOut], sizeof(T));
        }
    } 
    else 
    {
        int zero = 0;
        if (bWriteToMem)
        {
            memcpy(&pMem[offsetInOut], &zero, sizeof(T));
        }
    }

	offsetInOut += sizeof(T);
}

void MemorySerialize(std::string& num, uint8_t* pMem, int &offsetInOut, bool bWriteToMem);
void MemorySerializeRaw(uint8_t* pVar, uint8_t* pMem, int sizeBytes, int &offsetInOut, bool bWriteToMem);
bool MemorySerializeStringLarge(std::string& num, uint8_t* pMem, int &offsetInOut, bool bWriteToMem, uint32_t maxBytesInPacket = 0);
void MemorySerializeStringEncrypted(std::string& num, uint8_t* pMem, int& offsetInOut, bool bWriteToMem, int cryptID, const char* secretCode);

static std::string EncryptItemNameString(const std::string& num, const uint32_t& cryptID)
{
    constexpr std::string_view key{ "PBG892FXX982ABC*" };
    std::string ret(num.size(), 0);

    for (uint32_t i = 0; i < num.size(); i++)
    {
        ret[i] = static_cast<char>(num[i] ^ key[(i + cryptID) % key.size()]);
    }

    return ret;
}

static std::string GetRandomPart(const int& index);
static std::string base64_encode(const std::string& input)
{
    static constexpr char sEncodingTable[] =
    {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
    };

    size_t in_len = input.size();
    size_t out_len = 4 * ((in_len + 2) / 3);
    std::string ret(out_len, '\0');
    size_t i;
    char* p = &ret[0];

    for (i = 0; i < in_len - 2; i += 3)
    {
        *p++ = sEncodingTable[(input[i] >> 2) & 0x3F];
        *p++ = sEncodingTable[((input[i] & 0x3) << 4) | ((int)(input[i + 1] & 0xF0) >> 4)];
        *p++ = sEncodingTable[((input[i + 1] & 0xF) << 2) | ((int)(input[i + 2] & 0xC0) >> 6)];
        *p++ = sEncodingTable[input[i + 2] & 0x3F];
    }

    if (i < in_len)
    {
        *p++ = sEncodingTable[(input[i] >> 2) & 0x3F];
        if (i == (in_len - 1))
        {
            *p++ = sEncodingTable[((input[i] & 0x3) << 4)];
            *p++ = '=';
        }
        else
        {
            *p++ = sEncodingTable[((input[i] & 0x3) << 4) | ((int)(input[i + 1] & 0xF0) >> 4)];
            *p++ = sEncodingTable[((input[i + 1] & 0xF) << 2)];
        }

        *p++ = '=';
    }

    return ret;
}