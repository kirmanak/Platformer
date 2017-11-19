#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <structs.h>
#include <main.h>
#include <connect.h>

#include <SDL/SDL.h>

const level ask_level(char *);
void send_position(char *, SDL_Rect);

#endif
