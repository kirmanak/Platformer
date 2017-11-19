#ifndef _STRUTCTS_H_
#define _STRUTCTS_H_

#include <stdbool.h>

typedef struct input_condition {
  bool up;
  bool down;
  bool right;
  bool left;
  bool close;
} input_condition;

typedef struct limit {
  int up;
  int down;
  int right;
  int left;
} limit;

typedef struct level {
  int start_x;
  int start_y;
  int limits_size;
  limit *limits;
} level;

typedef struct client_condition {
  int x;
  int y;
  bool close;
} client_condition;

#endif 
