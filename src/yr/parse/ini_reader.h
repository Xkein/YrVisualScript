#pragma once

#include "yr/parse/parser.h"
#include <CCINIClass.h>

class IniReader
{
    CCINIClass* IniFile;
    static char _readBuffer[2048];

public:
    explicit IniReader(CCINIClass* pIni) : IniFile(pIni) {}

    char* value() const
    {
        return _readBuffer;
    }

    size_t max_size() const
    {
        return sizeof(_readBuffer);
    }

    bool empty() const
    {
        return !_readBuffer[0];
    }

    // basic string reader
    size_t ReadString(const char* pSection, const char* pKey)
    {
        auto const res = IniFile->ReadString(pSection, pKey, "", this->value(), this->max_size());
        return static_cast<size_t>(res);
    }

    template<typename T>
    bool Read(const char* pSection, const char* pKey, T& buffer)
    {
        if (this->ReadString(pSection, pKey))
        {
            return Parser<T>::Read(_readBuffer, buffer);
        }
        return false;
    }

};
