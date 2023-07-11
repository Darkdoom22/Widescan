#pragma once
#include <windows.h>
#include <memoryapi.h>
#include <cstdint>
#include <string>
#include "PebStructs.h"
namespace FindPattern
{
    char* TO_CHAR(wchar_t*);
    PEB* GetPEB();
    LDR_DATA_TABLE_ENTRY* GetLDREntry(std::string name);
    char* ScanBasic(char* pattern, char* mask, char* begin, intptr_t size);
    char* ScanInternal(char* pattern, char* mask, char* begin, intptr_t size);
    char* ScanModIn(char* pattern, char* mask, std::string modName);
}