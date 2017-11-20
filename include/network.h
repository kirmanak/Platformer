#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <structs.h>
#include <main.h>
#include <connect.h>

const level ask_level(int mysocket);

void send_state(int mysocket, client_condition cond);
const int server_connect (char *address);

ssize_t receive_buffer(size_t, unsigned char *, int);

#endif
