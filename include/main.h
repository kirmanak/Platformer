#ifndef _MAIN_H_
#define _MAIN_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH (960)
#define WINDOW_HEIGHT (540)
#define CHARACTER_WIDTH (64)
#define CHARACTER_HEIGHT (50)
#define WINDOW_TITLE "Platformer"
#define IMAGES_FOLDER "resources/"
#define IMAGE_NAME "_character.png"
#define CHARACTER_IMAGE(NUMBER) IMAGES_FOLDER #NUMBER IMAGE_NAME
#define CHARACTER_SPEED (200)
#define FPS 60
#define NUMBER_OF_SPRITES 1


#endif
