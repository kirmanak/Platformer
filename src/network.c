#include <network.h>
#include <serialize.h>
#include <connect.h>

#include <stdlib.h>

int receive_buffer(size_t size, unsigned char *const buffer, const int mysocket) {
  int len = recv(mysocket, buffer, size, 0);
  printf("Received %d bytes.\n", len);
  return len;
}

void send_position (char *address, SDL_Rect rect) {

}

const level ask_level(char *address) {
  unsigned char *buffer = malloc(sizeof(level));
  int mysocket;
  struct sockaddr_in dest;

  mysocket = socket(AF_INET, SOCK_STREAM, 0);

  memset(&dest, 0, sizeof(dest)); // zero the struct
  dest.sin_family = AF_INET;
  dest.sin_addr.s_addr = inet_addr(address); // set destination IP
  dest.sin_port = htons(PORTNUM); // set destination port number

  connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));

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
  buffer = calloc(lvl.limits_size, sizeof(limit));
  receive_buffer(lvl.limits_size * sizeof(limit), buffer, mysocket);

  lvl.limits = calloc(lvl.limits_size, sizeof(limit));
  for (int i = 0; i < lvl.limits_size; i++) {
    lvl.limits[i] = deserialize_limit(buffer + sizeof(limit) * i);
  }

  free(buffer);
  close(mysocket);
  return lvl;
}
