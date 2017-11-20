#include <network.h>
#include <serialize.h>

void send_state (const int mysocket, const client_condition cond) {
	unsigned char *buffer = malloc(sizeof(cond));
	serialize_client_condition(buffer, cond);
	send(mysocket, buffer, sizeof(int) * 2 + sizeof(bool), 0);
	free(buffer);
}

int receive_int(const int mysocket) {
	unsigned char *const buffer = malloc(sizeof(int));
	if (recv(mysocket, buffer, sizeof(int), 0) >= 0) {
		const int i = deserialize_int(buffer);
		free(buffer);
		return i;
	} else {
		free(buffer);
		return INT32_MIN;
	}
}

client_condition receive_client(const int mysocket) {
	unsigned char *const buffer = malloc(sizeof(client_condition));
	if (recv(mysocket, buffer, sizeof(client_condition), 0) >= 0) {
		const client_condition cond = deserialize_client_condition(buffer);
		free(buffer);
		return cond;
	} else {
		free(buffer);
		client_condition cond;
		cond.x = -1;
		cond.y = -1;
		cond.close = true;
		return cond;
	}
}

const int server_connect (char *address) {
  int mysocket;
  struct sockaddr_in dest;

  mysocket = socket(AF_INET, SOCK_STREAM, 0);

  memset(&dest, 0, sizeof(dest)); // zero the struct
  dest.sin_family = AF_INET;
  dest.sin_addr.s_addr = inet_addr(address); // set destination IP
  dest.sin_port = htons(PORTNUM); // set destination port number

	if (connect(mysocket, (struct sockaddr *) &dest, sizeof(struct sockaddr_in))) {
		return -1;
	} else {
		return mysocket;
	}
}

const level ask_level(const int mysocket) {
  unsigned char *buffer = malloc(sizeof(level));
  // receive level
	recv(mysocket, buffer, sizeof(level), 0);
  level lvl = deserialize_level(buffer);
	free(buffer);

	// receive limits
	const size_t limits_size = (size_t) lvl.limits_size * sizeof(limit);
	buffer = malloc(limits_size);
	recv(mysocket, buffer, limits_size, 0);

	lvl.limits = malloc(limits_size);
  for (int i = 0; i < lvl.limits_size; i++) {
    lvl.limits[i] = deserialize_limit(buffer + sizeof(limit) * i);
	  printf("Start position is (%d; %d), limit is (%d;%d;%d;%d)\n", lvl.start_x, lvl.start_y,
	         lvl.limits[0].up, lvl.limits[0].down, lvl.limits[0].right,
	         lvl.limits[0].left);

  }

  free(buffer);
  return lvl;
}

