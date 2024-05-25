/*
 *
 * author : yekuuun
 * Github : https://github.com/yekuuun
 *
 */

#include "CustomWinApi.hpp"


//////////////////////////////////////////////////////////////////////////////////////////////////
//  Equivalent to the Windows api function GetModuleHandleA
//////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Retrieves the address of an loaded module by name
/// </summary>
/// <param name="moduleName">name of the module</param>
/// <returns>returns address of the module in memory</returns>
HMODULE WINAPI GetModuleHandle(wchar_t* moduleName){
    if(moduleName == nullptr)
    {
        return nullptr;
    }

    PTIB ptrTib = (PTIB)GetTibAddress();
    PPEB ptrPeb = ptrTib->pPEB;

    if(ptrPeb == nullptr)
    {
        return nullptr;
    }

    //go to PEB_LDR_DATA;
    PPEB_LDR_DATA  ptrPebLdrData = ptrPeb->Ldr;

    //address loaded modules => first module of chained list
    PLIST_ENTRY ptrInMemoryModules = &ptrPebLdrData->InLoadOrderModuleList;

    //top loaded module
    PLIST_ENTRY moduleList = ptrInMemoryModules->Flink;

    //go through list of modules
    while(moduleList != ptrInMemoryModules)
    {
        //go into module loaded structs.
        auto ptrLdrDataTableEntry = (PLDR_DATA_TABLE_ENTRY)moduleList;
        if(StringContains(ptrLdrDataTableEntry->BaseDllName.buffer, moduleName))
        {
            return ptrLdrDataTableEntry->DllBase;
        }

        moduleList = moduleList->Flink;
    }

    return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//  Equivalent to the Windows api function GetProcAddress
//////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Retrieves the address of a function in a loaded module
/// </summary>
/// <param name="hModule">pointer to the loaded module</param>
/// <param name="procName">name of the function to retrieve</param>
/// <returns>returns address of the founded function</returns>
PVOID GetProcAddress(HMODULE hModule, LPCSTR procName){

    BYTE* dllAddress = (BYTE*)hModule;

    if(procName == nullptr)
    {
        return nullptr;
    }

    //ptr to IMAGE_DOS_HEADER
    auto ptrImageDosHeader = (PIMAGE_DOS_HEADER)hModule;
    if(ptrImageDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
    {
        return nullptr;
    }

    //ptr to IMAGE_NT_HEADER
    auto ptrImageNtHeader = (PIMAGE_NT_HEADERS64)(dllAddress + ptrImageDosHeader->e_lfanew);

    //ptr to IMAGE_OPTIONAL_HEADER
    PIMAGE_OPTIONAL_HEADER64 ptrImageOptionalHeader = &ptrImageNtHeader->OptionalHeader;

    //ptr to IMAGE_EXPORT_DIRECTORY
    auto ptrImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)hModule + ptrImageOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

    //address of AddressOfNames
    auto rvaNames = (DWORD*)(dllAddress + ptrImageExportDirectory->AddressOfNames);
    auto rvaOrdinalsNames = (WORD*)(dllAddress + ptrImageExportDirectory->AddressOfNameOrdinals);
    auto rvaFunction = (DWORD*)(dllAddress + ptrImageExportDirectory->AddressOfFunctions);

    //looping through names exported
    for(int i = 0; i < ptrImageExportDirectory->NumberOfNames; i++)
    {
        char* functionName = (char*)(dllAddress + rvaNames[i]);
        //compare strings
        if(CompareStringsA(functionName, procName))
        {
            return (LPVOID)(dllAddress + rvaFunction[rvaOrdinalsNames[i]]);
        }
    }

    return nullptr;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//  Retrieving address of current PEB
//////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Retrieves the address of the current PEB (process environment block)
/// </summary>
/// <returns>returns the address of the current process PEB</returns>
PVOID GetPebAddress(){
    return reinterpret_cast<PVOID>(__readgsqword(PEB_OFFSET));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//	Retrieving address of current TIB
//////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Retrieves the address of the current TIB
/// </summary>
/// <returns>returns the address of the current process PEB</returns>
PVOID GetTibAddress(){
    return reinterpret_cast<PVOID>(__readgsqword(TIB_OFFSET));
}

/*
 * GetProcessInformation
 * Listing process information
 * returns TRUE on function success
 */
BOOL GetProcessInformation(){
    wchar_t dllName[]      = L"ntdll.dll";
    ULONG sizePtr          = 0;
    ULONG bufferSize       = 0;
    LPVOID buffer          = nullptr;

    //getting address of NtQuerySystemInformation using custom GetModuleHandle() & GetProcAddress()
    PNTQUERYSYSTEMINFORMATION ptrNtQuerySystemInformation = (PNTQUERYSYSTEMINFORMATION)GetProcAddress(GetModuleHandle(dllName), "NtQuerySystemInformation");

    NTSTATUS status = ptrNtQuerySystemInformation(SystemProcessInformation, nullptr, sizePtr, &bufferSize);

    while(status != STATUS_SUCCESS)
    {
        buffer = malloc(bufferSize);
        sizePtr = bufferSize;
        if(buffer == nullptr)
        {
            std::cout << "[-] error allocating memory" << std::endl;
            return false;
        }
        status = ptrNtQuerySystemInformation(SystemProcessInformation, buffer, sizePtr, &bufferSize);
    }

    PSYSTEM_PROCESS_INFORMATION ptrProcessInfo = (PSYSTEM_PROCESS_INFORMATION)buffer;

    if(ptrProcessInfo == nullptr)
    {
        std::cout << "[-] nullptr for process informations" <<std::endl;
        return false;
    }

    while(ptrProcessInfo->NextEntryOffset)
    {
        std::cout << "[*] PROCESS ID : " << std::hex << ptrProcessInfo->UniqueProcessId << std::endl;

        ptrProcessInfo = (PSYSTEM_PROCESS_INFORMATION)((ULONG_PTR)ptrProcessInfo + ptrProcessInfo->NextEntryOffset);
    }

    free(buffer);
    return true;
}

/*
 * CustomStrLenA
 * returns length of a LPCSTR string
 */
size_t CustomStrLenA(LPCSTR str){
    LPCSTR ptrStr = str;
    while(*str != '\0')
    {
        str++;
    }
    return str - ptrStr;
}

/*
* StringContains
*
* Use: Case Insensitive String Contains Check
* Parameters: wchar_t string to search, wchar_t string to search for
* Return: Result of wchar_t equality
*/
BOOL StringContains(wchar_t* haystack, wchar_t* needle) {
    while (*haystack && (*haystack == *needle)) {
        haystack++;
        needle++;
    }
    return (*haystack == *needle);
}

/*
 * CompareStringsA
 * compare 2 strings
 */
BOOL CompareStringsA(LPCSTR str1, LPCSTR str2){
    if(str1 == nullptr || str2 == nullptr)
    {
        return false;
    }

    if(CustomStrLenA(str1) != CustomStrLenA(str2))
    {
        return false;
    }

    while(*str1 != '\0' && *str2 != '\0')
    {
        if(*str1 != *str2)
        {
            return false;
        }
        str2++;
        str1++;
    }
    return true;
}

//CustomWinApi project banner
VOID ShowBanner() {
    std::cout << "   ______           __                _       ___       ___    ____  _ \n"
                 "  / ____/_  _______/ /_____  ____ ___| |     / (_)___  /   |  / __ \\(_)\n"
                 " / /   / / / / ___/ __/ __ \\/ __ `__ \\ | /| / / / __ \\/ /| | / /_/ / / \n"
                 "/ /___/ /_/ (__  ) /_/ /_/ / / / / / / |/ |/ / / / / / ___ |/ ____/ /  \n"
                 "\\____/\\__,_/____/\\__/\\____/_/ /_/ /_/|__/|__/_/_/ /_/  |_/_/   /_/   \n";

    std::cout << "----------------------------------------------------------------------------\n" << std::endl;
}
