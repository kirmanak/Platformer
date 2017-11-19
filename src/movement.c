#include <movement.h>

void calculate_position(const input_condition condition,
                        SDL_Rect *const rect) {
  // calculating velocity
  int x_velocity = 0, y_velocity = 0;
  if (condition.up && !condition.down)
    y_velocity = -CHARACTER_SPEED;
  if (condition.down && !condition.up)
    y_velocity = CHARACTER_SPEED;
  if (condition.left && !condition.right)
    x_velocity = -CHARACTER_SPEED;
  if (condition.right && !condition.left)
    x_velocity = CHARACTER_SPEED;
  x_velocity /= FPS;
  y_velocity /= FPS;

  // updating position
  rect->x += x_velocity;
  rect->y += y_velocity;
}

void check_bounds(const level current_level, SDL_Rect *const rect) {
  for (int i = 0; i < current_level.limits_size; i++) {
    const limit current = current_level.limits[i];
    // detection bounds
    if (rect->x <= current.left)
      rect->x = current.left;
    if (rect->y <= current.up)
      rect->y = current.up;
    if (rect->x >= current.right - CHARACTER_WIDTH)
      rect->x = current.right - CHARACTER_WIDTH;
    if (rect->y >= current.down - CHARACTER_HEIGHT)
      rect->y = current.down - CHARACTER_HEIGHT;
  }
}

