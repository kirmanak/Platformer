#include <network.h>
#include <serialize.h>
#include <server.h>

#include <stdio.h>

int main(const int argc, const char *const *const argv) {
  unsigned char *buffer = calloc(1, sizeof(level));
  level current_level = generate_level();
  struct sockaddr_in dest; /* socket info about the machine connecting to us */
  struct sockaddr_in serv; /* socket info about our server */
  int mysocket;            /* socket used to listen for incoming connections */
  socklen_t socksize = sizeof(struct sockaddr_in);

  memset(&serv, 0,
         sizeof(serv));      /* zero the struct before filling the fields */
  serv.sin_family = AF_INET; /* set the type of connection to TCP/IP */
  serv.sin_addr.s_addr =
      htonl(INADDR_ANY);          /* set our address to any interface */
  serv.sin_port = htons(PORTNUM); /* set the server port number */

  mysocket = socket(AF_INET, SOCK_STREAM, 0);

  /* bind serv information to mysocket */
  bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));

  /* start listening, allowing a queue of up to 1 pending connection */
  listen(mysocket, 1);
  int consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);

  while (consocket) {
    printf("Incoming connection from %s - sending level...\n",
            inet_ntoa(dest.sin_addr));
    serialize_level(buffer, current_level);
    send(consocket, buffer, sizeof(level), 0);
    buffer = calloc(current_level.limits_size, sizeof(limit));
    for (int i = 0; i < current_level.limits_size; i++) {
      serialize_limit(buffer + sizeof(limit) * i, current_level.limits[i]);
    }
    printf("Sending limits...\n");
    send(consocket, buffer, sizeof(limit) * current_level.limits_size, 0);
    close(consocket);
    consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
  }

  free(current_level.limits);
  close(mysocket);
}

const level generate_level(void) {
  level lvl;
  lvl.start_x = 0;
  lvl.start_y = 0;
  lvl.limits_size = 1;
  lvl.limits = calloc(lvl.limits_size, sizeof(limit));
  lvl.limits[0].up = 0;
  lvl.limits[0].left = 0;
  lvl.limits[0].down = WINDOW_HEIGHT;
  lvl.limits[0].right = WINDOW_WIDTH;
  return lvl;
}
