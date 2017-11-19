#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <structs.h>
#include <main.h>
#include <connect.h>

const level ask_level(const int mysocket);
void send_state (const int mysocket, const client_condition cond);
const int server_connect (char *address);
int receive_buffer(size_t, unsigned char *const, const int);

#endif
