#pragma once
#include "NTHeader.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////
//		                                   Equivalent to the Windows api function GetModuleHandleA
//////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Retrieves the address of an loaded module by name
/// </summary>
/// <param name="moduleName">name of the module</param>
/// <returns>returns address of the module in memory</returns>
HMODULE WINAPI GetModuleHandle(wchar_t* moduleName);

//////////////////////////////////////////////////////////////////////////////////////////////////
//  Equivalent to the Windows api function GetProcAddress
//////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Retrieves the address of a function in a loaded module
/// </summary>
/// <param name="hModule">pointer to the loaded module</param>
/// <param name="procName">name of the function to retrieve</param>
/// <returns>returns address of the founded function</returns>
PVOID GetProcAddress(HMODULE hModule, LPCSTR procName);

//////////////////////////////////////////////////////////////////////////////////////////////////
//		                                                         Retrieving address of current PEB
//////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Retrieves the address of the current PEB (process environment block)
/// </summary>
/// <returns>returns the address of the current process PEB</returns>
PVOID GetPebAddress();

//////////////////////////////////////////////////////////////////////////////////////////////////
//		                                                         Retrieving address of current TIB
//////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Retrieves the address of the current TIB
/// </summary>
/// <returns>returns the address of the current process PEB</returns>
PVOID GetTibAddress();

/*
 * GetProcessInformation
 * Listing process information
 * returns TRUE on function success
 */
BOOL GetProcessInformation();

/*
* StringContains
*
* Use: Case Insensitive String Contains Check
* Parameters: wchar_t string to search, wchar_t string to search for
* Return: Result of wchar_t equality
*/
BOOL StringContains(wchar_t* haystack, wchar_t* needle);

/*
 * CompareStringsA
 * compare 2 strings
 */
BOOL CompareStringsA(LPCSTR str1, LPCSTR str2);

VOID ShowBanner();