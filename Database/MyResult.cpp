#ifndef MYSQLRESULT
#define MYSQLRESULT
#include <string>
#include <istream>
#include <sstream>
#include <ostream>
#include <cppconn/resultset.h>

class MyResult
{
public:
    MyResult(sql::ResultSet* pResult)
    {
        m_pResult = pResult;
    }
    sql::ResultSet* Get() { return m_pResult; }


    bool GetBool(const uint32_t& colum_index)
    {
        return m_pResult->getInt(colum_index) == 1 ? true : false;
    }

    bool GetBool(const std::string& colum)
    {
        return m_pResult->getInt(colum.c_str()) == 1 ? true : false;
    }

    uint8_t GetByte(const uint32_t& colum_index)
    {
        return (uint8_t)m_pResult->getInt(colum_index);
    }

    uint8_t GetByte(const std::string& colum)
    {
        return (uint8_t)m_pResult->getInt(colum.c_str());
    }

    int GetInt(const uint32_t& colum_index)
    {
        return m_pResult->getInt(colum_index);
    }

    int GetInt(const std::string& colum)
    {
        return m_pResult->getInt(colum.c_str());
    }

    unsigned int GetUnsignedInt(const std::string& colum)
    {
        return m_pResult->getUInt(colum.c_str());
    }

    uint32_t GetUInt(const uint32_t& colum_index)
    {
        return (uint32_t)m_pResult->getInt(colum_index);
    }

    uint32_t GetUInt(const std::string& colum)
    {
        return (uint32_t)m_pResult->getInt(colum.c_str());
    }

    uint64_t GetLongLong(const uint32_t& colum_index)
    {
        return m_pResult->getUInt64(colum_index);
    }

    uint64_t GetLongLong(const std::string& colum)
    {
        return m_pResult->getUInt64(colum.c_str());
    }

    double GetDouble(const uint32_t& colum_index)
    {
        return m_pResult->getDouble(colum_index);
    }

    double GetDouble(const std::string& colum)
    {
        return m_pResult->getDouble(colum.c_str());
    }

    std::string GetString(const std::string& colum)
    {
        std::istream* val = m_pResult->getBlob(colum.c_str());
        char val_char[0xFF + 1];
        val->getline(val_char, 0xFF);
        return std::string(val_char);
    }

    std::string GetString(const uint32_t& colum_index)
    {
        std::istream* val = m_pResult->getBlob(colum_index);
        char val_char[0xFF + 1];
        val->getline(val_char, 0xFF);
        return std::string(val_char);
    }

    uint8_t* GetBuffer(const std::string& column, size_t& bufferSize)
    {
        std::istream* blobStream = m_pResult->getBlob(column.c_str());
        if (!blobStream)
        {
            bufferSize = 0;
            return nullptr;
        }

        blobStream->seekg(0, std::ios::end);
        bufferSize = blobStream->tellg();
        blobStream->seekg(0, std::ios::beg);

        uint8_t* pBuf = new uint8_t[bufferSize];
        blobStream->read(reinterpret_cast<char*>(pBuf), bufferSize);
        return pBuf;
    }

private:
    sql::ResultSet* m_pResult = NULL;

};

#endif