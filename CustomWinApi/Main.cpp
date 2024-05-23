/*
 *
 * author : yekuuun
 * Github : https://github.com/yekuuun
 *
 */
#include "CustomWinApi.hpp"

int main(){
    #ifdef _WIN64
        ShowBanner();
        std::wcout.imbue(std::locale(""));

        wchar_t dllName[] = L"ntdll.dll";

        ///calling GetModuleHandle for this example.
        HMODULE hModule = GetModuleHandle(dllName);
        LPCSTR procName = "NtQuerySystemInformation";

        if (hModule != nullptr)
        {
            std::wcout << L"GetModuleHandle found " << dllName << " at :" << std::hex << hModule << std::endl;
        } else {
            std::wcout << dllName << " was not found..." << std::endl;
        }
        ///---------------------------------------------

        ///calling GetProcAddress
        LPVOID procAddress = GetProcAddress(hModule, procName);
        if(procAddress == nullptr)
        {
            std::cout << procName << " was not found" << std::endl;
        }
        else
        {
            std::cout << procName << " was found at : " << std::hex << procAddress << std::endl;
        }

        ///---------------------------------------------

        return EXIT_SUCCESS;
    #else
        std::cout << "code designed for x64" << std::end;
        return EXIT_FAILURE;
    #endif
}
