/**
 * headers-commons.h
 *
 * Realiza las inclusiones comunes de varios headers del 
 * sistema utilizados por los otros fuentes/headers.
 *
 */



#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <process.h>
#include <ws2tcpip.h>
#include <string.h>
#include <tchar.h>
#include <strsafe.h>
#include <time.h>
#include <signal.h>
#pragma comment (lib, "Ws2_32.lib")
