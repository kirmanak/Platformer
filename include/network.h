#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <structs.h>
#include <main.h>
#include <connect.h>

const level ask_level(int mysocket);

void send_state(int, client_condition);

const int server_connect(char *);

int receive_int(int);

client_condition receive_client(int);

#endif
