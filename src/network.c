#include <network.h>
#include <serialize.h>
#include <connect.h>

#include <stdlib.h>

int receive_buffer(size_t size, unsigned char *const buffer, const int mysocket) {
  int len = recv(mysocket, buffer, size, 0);
  printf("Received %d bytes.\n", len);
  return len;
}

const level ask_level(void) {
  unsigned char *buffer = malloc(sizeof(level));
  int mysocket;
  struct sockaddr_in dest;

  mysocket = socket(AF_INET, SOCK_STREAM, 0);

  memset(&dest, 0, sizeof(dest)); // zero the struct
  dest.sin_family = AF_INET;
  dest.sin_addr.s_addr = htonl(
      INADDR_LOOPBACK); // set destination IP number - localhost, 127.0.0.1
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
  printf("Level is %d, %d, %d\n", lvl.start_x, lvl.start_y, lvl.limits_size); 

  // receive limits
  free(buffer);
  buffer = calloc(lvl.limits_size, sizeof(limit));
  receive_buffer(lvl.limits_size * sizeof(limit), buffer, mysocket);

  lvl.limits = calloc(lvl.limits_size, sizeof(limit));
  for (int i = 0; i < lvl.limits_size; i++) {
    lvl.limits[i] = deserialize_limit(buffer + sizeof(limit) * i);
    // printf("Limit is %d, %d, %d, %d\n", lvl.limits[i].up, lvl.limits[i].down, lvl.limits[i].right, lvl.limits[i].left); 
  }

  free(buffer);
  close(mysocket);
  return lvl;
}
