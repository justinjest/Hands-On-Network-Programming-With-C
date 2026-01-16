#ifndef CHAP04_H_
#define CHAP04_H_

#if defined(_WIN32)
#ifndef _WIND32_WINNT
#define _WIN32_WINNT 0x0600
#endif // ifndef __WIN32_WINNT
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#endif // if defined(_WIN32)

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

#endif // if defined(_WIN32)

#include <stdio.h>
#include <string.h>

#endif // CHAP04_H_
