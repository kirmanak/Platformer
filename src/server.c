#include <network.h>
#include <serialize.h>
#include <server.h>

static int n;
static int *sockets;
static client_condition *conditions;

void forked_process(int);

void send_clients(int);

int main(const int argc, const char *const *const argv) {
  unsigned char *buffer = calloc(1, sizeof(level));
	n = 0;
	sockets = calloc(1, sizeof(int));
  level current_level = generate_level();
  struct sockaddr_in dest; // socket info about the machine connecting to us
  struct sockaddr_in serv; // socket info about our server
  int mysocket;            // socket used to listen for incoming connections
  socklen_t socksize = sizeof(struct sockaddr_in);

  memset(&serv, 0, sizeof(serv)); // zero the struct before filling the fields
  serv.sin_family = AF_INET; // set the type of connection to TCP/IP
  serv.sin_addr.s_addr = htonl(INADDR_ANY); // set our address to any interface
  serv.sin_port = htons(PORTNUM); // set the server port number

  mysocket = socket(AF_INET, SOCK_STREAM, 0);

  // bind serv information to mysocket
  bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));

  // start listening, allowing a queue of up to 1 pending connection
	listen(mysocket, 5);
  int consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);

  while (consocket) {
    printf("Incoming connection from %s - sending level...\n",
            inet_ntoa(dest.sin_addr));
	  n++;
	  sockets = realloc(sockets, n * sizeof(int));
	  sockets[n - 1] = consocket;
    serialize_level(buffer, current_level);
    send(consocket, buffer, sizeof(level), 0);
    free(buffer);
    buffer = calloc((size_t) current_level.limits_size, sizeof(limit));
	  for (int i = 0; i < current_level.limits_size; i++) {
      serialize_limit(buffer + sizeof(limit) * i, current_level.limits[i]);
    }
    send(consocket, buffer, sizeof(limit) * current_level.limits_size, 0);
	  const pid_t main_thread = getpid();
    fork();
    if (getpid() != main_thread) {
      forked_process(consocket);
    }
    consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
  }

	free(sockets);
	sockets = NULL;
  free(buffer);
	buffer = NULL;
  free(current_level.limits);
	current_level.limits = NULL;
  close(mysocket);
}

void send_clients(const int consocket) {
	unsigned char *const serialized_n = malloc(sizeof(int));
	unsigned char *const serialized_cond = malloc(sizeof(client_condition));
	serialize_int(serialized_n, n);
	for (int i = 0; i < n; i++) {
		if (send(consocket, serialized_n, sizeof(int), 0) == -1) {
			printf("Error during sending\n");
		} else {
			printf("Sent %d\n", n);
		}
		for (int j = 0; j < n; j++) {
			if (sockets[i] == consocket) continue;
			serialize_client_condition(serialized_cond, conditions[j]);
			send(sockets[i], serialized_cond, sizeof(client_condition), 0);
		}
	}
}

void forked_process(const int consocket) {
	const size_t size = sizeof(client_condition);
  unsigned char *buff = malloc(size);
	// receive buffer with character position and process it
	while (true) {
		if (recv(consocket, buff, size, 0) <= 0) break;
    client_condition cond = deserialize_client_condition(buff);
		printf("Cond is %d, %d, %d\n", cond.x, cond.y, cond.close);
		for (int i = 0; i < n; i++) {
			if (consocket != sockets[i]) continue;
			conditions[i] = cond;
		}
		// here sends to all clients
		send_clients(consocket);
		printf("I've sent all clients conditions to all clients.");
    if (cond.close) break;
  }
	close(consocket);
  free(buff);
}

const level generate_level(void) {
  level lvl;
  lvl.start_x = 0;
  lvl.start_y = 0;
  lvl.limits_size = 1;
  lvl.limits = calloc((size_t) lvl.limits_size, sizeof(limit));
  lvl.limits[0].up = 0;
  lvl.limits[0].left = 0;
  lvl.limits[0].down = WINDOW_HEIGHT;
  lvl.limits[0].right = WINDOW_WIDTH;
  return lvl;
}
