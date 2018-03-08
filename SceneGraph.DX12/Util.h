#pragma once

#include <stdexcept>
#include <string>
#include <Windows.h>

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        // Defina um ponto de interrupção nesta linha para detectar erros da API do Win32.
        LPWSTR pBuffer;
        int len;

        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, hr, 0, (LPWSTR)&pBuffer, 0, nullptr);

        // Determine the required size
        len = WideCharToMultiByte(CP_UTF8, 0, pBuffer, -1, nullptr, 0, nullptr, nullptr);

        LPSTR pStr = new CHAR[len + 1];
        pStr[len] = 0;

        WideCharToMultiByte(CP_UTF8, 0, pBuffer, -1, pStr, len, nullptr, nullptr);

        std::string what(pStr);

        delete[] pStr;

        throw std::runtime_error(what);
    }
}
