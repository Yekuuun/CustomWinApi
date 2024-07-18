# CustomWinApi 📦

This repo contains base code for reimplementing a custom version of GetModuleHandle( ) & GetProcAddress( ) functions without any imports crafting all headers by my hand using online ressources & github repos.
I used process hacker to check my code & address returned by GetModuleHandle( ) & GetProcAddress( ).

## Running program 

`git clone https://github.com/Yekuuun/CustomWinApi.git`

`cd /CustomWinApi`

`g++ Main.cpp CustomWinApi.cpp -o CustomWinApi -m64`

`./CustomWinApi`

---

## Functions demo
<img src="https://github.com/Yekuuun/CustomWinApi/blob/main/assets/Functions.png"></img>

### GetModuleHandle( ) && GetProcAddress( )

<img src="https://github.com/Yekuuun/CustomWinApi/blob/main/assets/GetModuleHandle.png"></img>

### Getting NtQuerySystemInformation to list processes 

<img src="https://github.com/Yekuuun/CustomWinApi/blob/main/assets/ListProcess.png"></img>

---

## Base windows functions declaration :

[WINDOWS GetProcAddress](https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress) <br> <br>
[WINDOWS GetModuleHandle](https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandlew)

---

## Greetings

- [ARSIUM](https://github.com/arsium) for NTHeader.hpp & windows internals advices
