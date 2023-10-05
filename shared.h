#pragma once

#include <iostream>

// windows headers
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib") // this indicates the WinSock2 DLL is needed. Do not remove.
enum MessageType {
    STRING = 0,
    NUMBER = 1,
    GOODBYE = ~0
};