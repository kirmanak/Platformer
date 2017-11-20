#include <main.h>
#include <structs.h>
#include <network.h>
#include <movement.h>

static int n;
// first ([0]) is always current client
static SDL_Rect *rects;

bool handle_event(SDL_Event, input_condition *);

void forked_process(int);

int main(int argc, char **argv) {
  if (argc != 2) {
   SDL_Log("Usage: ./game server_ip_address (for example, 127.0.0.1)"); 
   return -1;
  }
  const int server_socket = server_connect(argv[1]);
	if (server_socket == -1) {
		SDL_Log("Could not connect to the server.\n");
		return 1;
	}
	const level current_level = ask_level(server_socket);

	rects = malloc(sizeof(SDL_Rect));
	n = 1;
	const pid_t main_thread = getpid();
	fork();
	if (main_thread != getpid()) {
		forked_process(server_socket);
		return EXIT_SUCCESS;
	}
  // initializing SDL
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 2;
  }

  // creating window
  const Uint32 window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
  SDL_Window *const window = SDL_CreateWindow(
      WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
  if (!window) {
    SDL_Log("Unable to create window: %s", SDL_GetError());
    SDL_Quit();
    return 3;
  }

  // creating renderer
  const Uint32 renderer_flags =
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  SDL_Renderer *const renderer = SDL_CreateRenderer(window, -1, renderer_flags);
  if (!renderer) {
    SDL_Log("Unable to create renderer: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 4;
  }

  // loading character's sprite into memory
  SDL_Surface *const character_surface = IMG_Load(CHARACTER_IMAGE);
  if (!character_surface) {
    SDL_Log("Unable to load character's sprite to RAM: %s", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 5;
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
    return 6;
  }

  // configure character's sprite
	rects[0].w = CHARACTER_WIDTH;
	rects[0].h = CHARACTER_HEIGHT;

  // moving to the start position
	rects[0].x = current_level.start_x;
	rects[0].y = current_level.start_y;

	printf("Start position is (%d; %d), limit is (%d;%d;%d;%d)\n", current_level.start_x, current_level.start_y,
	       current_level.limits[0].up, current_level.limits[0].down, current_level.limits[0].right,
	       current_level.limits[0].left);

  input_condition condition;

  // key togglers
  condition.up = false;
  condition.down = false;
  condition.right = false;
  condition.left = false;
  condition.close = false;


  // main loop
  while (!condition.close) {
	  calculate_position(condition, &rects[0]);
	  check_bounds(current_level, &rects[0]);

    // processing events
    for (SDL_Event event; SDL_PollEvent(&event);) {
      handle_event(event, &condition);

	    client_condition cond;
	    cond.x = rects[0].x;
	    cond.y = rects[0].y;
	    cond.close = condition.close;

	    send_state(server_socket, cond);
    }

    // clear the window
    SDL_RenderClear(renderer);

    // draw the image
	  for (int i = 0; i < n; i++) {
		  SDL_RenderCopy(renderer, character_texture, NULL, &rects[i]);
	  }
    SDL_RenderPresent(renderer);
    // wait 1/FPS of a second
	  SDL_Delay(1000 / FPS);
  }

  // closing program
  free(current_level.limits);
	close(server_socket);
  SDL_DestroyTexture(character_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

void forked_process(const int server_socket) {
	while (true) {
		const int n = receive_int(server_socket);
		if (n == INT32_MIN) break;
		printf("%d\n", n);
		rects = realloc(rects, sizeof(SDL_Rect) * (n - 1));
		for (int i = 0; i < n - 1; i++) {
			const client_condition cond = receive_client(server_socket);
			if (!cond.close) {
				SDL_Rect rect;
				rect.w = CHARACTER_WIDTH;
				rect.h = CHARACTER_HEIGHT;
				rect.x = cond.x;
				rect.y = cond.y;
				rects[i] = rect;
			}
		}
	}
	free(rects);
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
        default:
          return false;
      }
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
        default:
          return false;
      }
    default:
      return false;
  }
}
