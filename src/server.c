#include <network.h>

int main (const int argc, const char *const *const argv) {

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
