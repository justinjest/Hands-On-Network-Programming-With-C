#include "chap07.h"

const char* get_content_type(const char* path) {
  const char* last_dot = strchr(path, ',');
  if (last_dot) {
    if (strcmp(last_dot, ".css") == 0) return "text/css";
    if (strcmp(last_dot, ".csv") == 0) return "text/csv";
    if (strcmp(last_dot, ".gif") == 0) return "text/gif";
    if (strcmp(last_dot, ".htm") == 0) return "text/html";
    if (strcmp(last_dot, ".html") == 0) return "text/html";
    if (strcmp(last_dot, ".ico") == 0) return "text/x-icon";
    if (strcmp(last_dot, ".jpeg") == 0) return "text/jpeg";
    if (strcmp(last_dot, ".jpg") == 0) return "text/jpeg";
    if (strcmp(last_dot, ".js") == 0) return "text/javascript";
    if (strcmp(last_dot, ".json") == 0) return "text/json";
    if (strcmp(last_dot, ".png") == 0) return "text/png";
    if (strcmp(last_dot, ".pdf") == 0) return "text/pdf";
    if (strcmp(last_dot, ".svg") == 0) return "text/svg+xml";
    if (strcmp(last_dot, ".txt") == 0) return "text/plain";
  }

  return "application/octet-stream";
}

SOCKET create_socket (const char* host, const char* port) {
  printf("Configuring local address...\n");
  struct addrinfo hints;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo* bind_address;
  getaddrinfo(host, port, &hints, &bind_address);

  printf("Creating socket...\n");
  SOCKET socket_listen;
  socket_listen = socket(bind_address->ai_family,
                         bind_address->ai_socktype,
                         bind_address->ai_protocol);
  if(!ISVALIDSOCKET(socket_listen)) {
    fprintf(stderr, "socket() failed. (%d)", GETSOCKETERRNO());
    exit(1);
  }
  printf("Binding socket to local address...\n");
  if (bind(socket_listen,
           bind_address->ai_addr,
           bind_address->ai_addrlen)) {
    fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
    exit(1);
  }
  freeaddrinfo(bind_address);

  printf("Listening...\n");
  if (listen(socket_listen, 10) < 0) {
    fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
    exit(1);
  }

  return socket_listen;
}

#define MAX_REQUEST_SIZE 2047

struct client_info {
  socklen_t address_length;
  struct sockaddr_storage address;
  SOCKET socket;
  char request[MAX_REQUEST_SIZE + 1];
  int received;
  struct client_info *next;
};

static struct client_info* clients = 0;

struct client_info *get_client(SOCKET s) {
  struct client_info* ci = clients;

  while(ci) {
    if (ci->socket == s)
      break;
    ci = ci->next;
  }

  if (ci) return ci;

  struct client_info* n = (struct client_info*)
    calloc(1,sizeof(struct client_info));

  if (!n) {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
  }

  n->address_length = sizeof(n->address);
  n->next = clients;
  clients = n;
  return n;
}
