#include <main.h>
#include <structs.h>
#include <movement.h>

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

bool handle_event(const SDL_Event, input_condition *const);
level create_level(void);

int main(const int argc, const char *const *const argv) {
  // initializing SDL
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  // creating window
  const Uint32 window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
  SDL_Window *const window = SDL_CreateWindow(
      WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
  if (!window) {
    SDL_Log("Unable to create window: %s", SDL_GetError());
    SDL_Quit();
    return 2;
  }

  // creating renderer
  const Uint32 renderer_flags =
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  SDL_Renderer *const renderer = SDL_CreateRenderer(window, -1, renderer_flags);
  if (!renderer) {
    SDL_Log("Unable to create renderer: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 3;
  }

  // loading character's sprite into memory
  SDL_Surface *const character_surface = IMG_Load(CHARACTER_IMAGE);
  if (!character_surface) {
    SDL_Log("Unable to load character's sprite to RAM: %s", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 4;
  }

  // loading sprite into graphics hardware's memory
  SDL_Texture *const character_texture =
      SDL_CreateTextureFromSurface(renderer, character_surface);
  SDL_FreeSurface(character_surface);
  if (!character_texture) {
    SDL_Log("Unable to load character's sprite to graphics hardware: %s",
            SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 5;
  }

  // struct to hold the position and size of the sprite
  SDL_Rect character_rectangle;

  // configure character's sprite
  SDL_QueryTexture(character_texture, NULL, NULL, &character_rectangle.w,
                   &character_rectangle.h);
  character_rectangle.w = CHARACTER_WIDTH;
  character_rectangle.h = CHARACTER_HEIGHT;

  level current_level = create_level();

  // moving to the start position
  character_rectangle.x = current_level.start_x;
  character_rectangle.y = current_level.start_y;

  input_condition condition;

  // key togglers
  condition.up = false;
  condition.down = false;
  condition.right = false;
  condition.left = false;
  condition.close = false;

  // main loop
  while (!condition.close) {
    // processing events
    for (SDL_Event event; SDL_PollEvent(&event);) {
      if (!handle_event(event, &condition)) continue;
    }
    calculate_position(condition, &character_rectangle);
    check_bounds(current_level, &character_rectangle);

    // clear the window
    SDL_RenderClear(renderer);

    // draw the image
    SDL_RenderCopy(renderer, character_texture, NULL, &character_rectangle);
    SDL_RenderPresent(renderer);
    // wait 1/FPS of a second
    SDL_Delay((double) 1000 / FPS);
  }

  // closing program
  free(current_level.limits);
  SDL_DestroyTexture(character_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

bool handle_event(const SDL_Event event, input_condition *const condition) {
  switch (event.type) {
    case SDL_QUIT:
      condition->close = true;
      return true;
    case SDL_KEYDOWN:
      switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_UP:
          condition->up = true;
          return true;
        case SDL_SCANCODE_S: case SDL_SCANCODE_DOWN:
          condition->down = true;
          return true;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
          condition->right = true;
          return true;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
          condition->left = true;
          return true;
      }
      return true;
    case SDL_KEYUP:
      switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_UP:
          condition->up = false;
          return true;
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
          condition->down = false;
          return true;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
          condition->right = false;
          return true;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
          condition->left = false;
          return true;
      }
      return true;
  }
  return false;
}

level create_level(void) {
  level lvl;
  lvl.start_x = 0;
  lvl.start_y = 0;
  lvl.limits_size = 1;
  lvl.limits = calloc(lvl.limits_size, sizeof(limit));
  lvl.limits[0].up = 0;
  lvl.limits[0].left = 0;
  lvl.limits[0].down = WINDOW_HEIGHT;
  lvl.limits[0].right = WINDOW_WIDTH;
  const limit current = lvl.limits[0];
  return lvl;
}

