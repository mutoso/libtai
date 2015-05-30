/* (c) 2015 Alastair Feille
   MIT License */

#ifndef TAI_H
#define TAI_H

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
    // You have to have WinSock2 before windows.h or you compiler errors (https://stackoverflow.com/questions/11726958/cant-include-winsock2-h-in-msvc-2010)
    #include <WinSock2.h>
    #include <windows.h>

    // Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib for WinSock2
    #pragma comment (lib, "Ws2_32.lib")
    #pragma comment (lib, "Mswsock.lib")
    #pragma comment (lib, "AdvApi32.lib")

    #define timegm _mkgmtime
#else
    #include <arpa/inet.h>
#endif

#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#define ntohll(x) htonll(x) // ntohll is also its own inverse
#define isleap(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)

typedef int64_t tai_t;
typedef struct {
    tai_t sec;
    uint32_t ns;
} tain_t;

tai_t tai_now();
tain_t tain_now();
tai_t tai_mktime(struct tm* date);
struct tm* tai_utctime(tai_t time);
tain_t tain_mktime(struct tm* date, uint32_t ns);
uint64_t tai_tobytes(tai_t time);
tai_t tai_frombytes(uint64_t time);
tain_t tain_tobytes(tain_t time);
tain_t tain_frombytes(tain_t time);

#endif
