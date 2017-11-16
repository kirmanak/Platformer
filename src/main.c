#include <params.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

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

  // moving to the center
  double character_x = (WINDOW_WIDTH - character_rectangle.w) / 2;
  double character_y = (WINDOW_HEIGHT - character_rectangle.h) / 2;

  // sprite moving speed
  double character_x_velocity = 0;
  double character_y_velocity = 0;

  // key togglers
  bool up = false;
  bool down = false;
  bool right = false;
  bool left = false;

  // main loop
  bool close_requested = false;
  while (!close_requested) {
    // processing events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          close_requested = true;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
              up = true;
              break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
              down = true;
              break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
              right = true;
              break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
              left = true;
              break;
          }
          break;
        case SDL_KEYUP:
          switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
              up = false;
              break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
              down = false;
              break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
              right = false;
              break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
              left = false;
              break;
          }
          break;
      }
    }
    // calculating velocity
    character_x_velocity = character_y_velocity = 0;
    if (up && !down) character_y_velocity = -CHARACTER_SPEED;
    if (down && !up) character_y_velocity = CHARACTER_SPEED;
    if (left && !right) character_x_velocity = -CHARACTER_SPEED;
    if (right && !left) character_x_velocity = CHARACTER_SPEED;

    // updating position
    character_x += character_x_velocity / 60;
    character_y += character_y_velocity / 60;

    // detection bounds
    if (character_x <= 0) character_x = 0;
    if (character_y <= 0) character_y = 0;
    if (character_x >= WINDOW_WIDTH - character_rectangle.w)
      character_x = WINDOW_WIDTH - character_rectangle.w;
    if (character_y >= WINDOW_HEIGHT - character_rectangle.h)
      character_y = WINDOW_HEIGHT - character_rectangle.h;

    // setting position in the struct
    character_rectangle.y = (int)character_y;
    character_rectangle.x = (int)character_x;

    // clear the window
    SDL_RenderClear(renderer);

    // draw the image
    SDL_RenderCopy(renderer, character_texture, NULL, &character_rectangle);
    SDL_RenderPresent(renderer);

    // wait 1/60th of a second
    SDL_Delay(1000 / 60);
  }

  // closing program
  SDL_DestroyTexture(character_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
