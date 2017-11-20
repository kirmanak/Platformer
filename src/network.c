#include <network.h>
#include <serialize.h>

ssize_t receive_buffer(size_t size, unsigned char *const buffer, const int mysocket) {
    ssize_t len = recv(mysocket, buffer, size, 0);
    printf("Received %zi bytes.\n", len);
  return len;
}

void send_state (const int mysocket, const client_condition cond) {
    unsigned char *buffer = malloc(sizeof(cond));
    serialize_client_condition(buffer, cond);
    send(mysocket, buffer, sizeof(int) * 2 + sizeof(bool), 0);
 free(buffer);
}

const int server_connect (char *address) {
  int mysocket;
  struct sockaddr_in dest;

  mysocket = socket(AF_INET, SOCK_STREAM, 0);

  memset(&dest, 0, sizeof(dest)); // zero the struct
  dest.sin_family = AF_INET;
  dest.sin_addr.s_addr = inet_addr(address); // set destination IP
  dest.sin_port = htons(PORTNUM); // set destination port number

  connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));

  return mysocket;
}

const level ask_level(const int mysocket) {
  unsigned char *buffer = malloc(sizeof(level));
  // receive level
  if (receive_buffer(sizeof(level), buffer, mysocket) <= 0) {
    level lvl;
    lvl.start_x = -1;
    lvl.start_y = -1;
    lvl.limits_size = -1;
    lvl.limits = NULL;
    return lvl;
  }
  level lvl = deserialize_level(buffer);

  // receive limits
  free(buffer);
    buffer = calloc((size_t) lvl.limits_size, sizeof(limit));
  receive_buffer(lvl.limits_size * sizeof(limit), buffer, mysocket);

    lvl.limits = calloc((size_t) lvl.limits_size, sizeof(limit));
  for (int i = 0; i < lvl.limits_size; i++) {
    lvl.limits[i] = deserialize_limit(buffer + sizeof(limit) * i);
  }

  free(buffer);
  close(mysocket);
  return lvl;
}

