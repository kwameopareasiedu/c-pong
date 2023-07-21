#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "window.c"
#include "game.c"

unsigned int last_frame_time = 0;

void start_game_loop() {
  if (game_running) {
    setup();

    while (game_running) {
      if (ENABLE_FRAME_RATE_CAP) {
        int idle_time = TARGET_FRAME_TIME - (SDL_GetTicks() - last_frame_time);

        if (idle_time > 0 && idle_time <= TARGET_FRAME_TIME)
          SDL_Delay(idle_time);
      }

      float delta_time = (float) (SDL_GetTicks() - last_frame_time) / 1000.0f;
      last_frame_time = SDL_GetTicks();

      process_input();
      update(delta_time);
      render();
    }
  }
}

int main() {
  game_running = create_window();
  start_game_loop();
  destroy_window();
  return 0;
}
