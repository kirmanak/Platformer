#include <network.h>
#include <serialize.h>
#include <connect.h>

#include <stdlib.h>

const level ask_level(void) {
  unsigned char *buffer = calloc(1, sizeof(level));
  int len, mysocket;
  struct sockaddr_in dest;

  mysocket = socket(AF_INET, SOCK_STREAM, 0);

  memset(&dest, 0, sizeof(dest)); /* zero the struct */
  dest.sin_family = AF_INET;
  dest.sin_addr.s_addr = htonl(
      INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1*/
  dest.sin_port = htons(PORTNUM); /* set destination port number */

  connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));

  len = recv(mysocket, buffer, sizeof(level), 0);

  /* We have to null terminate the received data ourselves */
  buffer[len] = '\0';

  printf("Received %s (%d bytes).\n", buffer, len);

  close(mysocket);
  return deserialize_level(buffer);
}
