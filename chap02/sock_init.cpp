/* sock_init.cpp */
// Provides default initialization for berkely and winsock
// Should compile on every device

#if defined(_WIN32)
// Windows definitions and bindings
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define ISVALIDSOCKET(s) ((S) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#else
// unix like definitions and bindings
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include<errno.h>

#define SOCKET int
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(S) close(s)
#define GETSOCKETERRNO() (errno)

#endif
// Unique macros that should work across all OS's:
// ISVALIDSOCKET -> returns true if socket is valid
// CLOSESOCKET   -> Closes socket
// GETSOCKEERRNO -> Returns last error number that we recieved

#include <stdio.h>

int main() {
  #if defined(_WIN32)
  WSADATA d;
  if (WSAStartup(MAKEWORD(2, 2), &d)) {
    fprintf(stderr, "Failed to initialize.\n");
    return 1;
  }
  #endif
  printf("Ready to use socket API.\n");

  #if defined(_WIN32)
  WSACleanup();
  #endif

  return 0;
}
