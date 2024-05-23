```                
   ______           __                _       ___       ___          _ 
  / ____/_  _______/ /_____  ____ ___| |     / (_)___  /   |  ____  (_)
 / /   / / / / ___/ __/ __ \/ __ `__ \ | /| / / / __ \/ /| | / __ \/ / 
/ /___/ /_/ (__  ) /_/ /_/ / / / / / / |/ |/ / / / / / ___ |/ /_/ / /  
\____/\__,_/____/\__/\____/_/ /_/ /_/|__/|__/_/_/ /_/_/  |_/ .___/_/   
                                                          /_/                                              
```

this repo contains base code for reimplementing a custom version of GetModuleHandle( ) & GetProcAddress( ) functions without any imports crafting all headers by my hand using online ressources & github repos.

## Running program 

`g++ Main.cpp CustomWinApi.cpp -o CustomWinApi -m64`


## GetModuleHandle( ) & GetProcAddress( )

Base functions declaration :

[WINDOWS GetProcAddress](https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress)
[WINDOWS GetModuleHandle](https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandlew)

### Greetings

- [ARSIUM](https://github.com/arsium) for NTHeader.hpp & windows internals advices
