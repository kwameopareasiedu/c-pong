/* Defines window management functions */
#include "config.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *cloud_font = NULL, *louis_george_cafe_font = NULL;
static SDL_Color white = {255, 255, 255, 255};

int create_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    fprintf(stderr, "SDL initialization failed\n");
    return FALSE;
  }

  window = SDL_CreateWindow(
      APP_NAME,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      0
  );

  if (!window) {
    fprintf(stderr, "SDL create window failed\n");
    return FALSE;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer) {
    fprintf(stderr, "SDL create renderer failed\n");
    return FALSE;
  }

  if (TTF_Init()) {
    fprintf(stderr, "SDL TTF initialization failed\n");
    return FALSE;
  }

  cloud_font = TTF_OpenFont("assets/cloud.ttf", 32);
  louis_george_cafe_font = TTF_OpenFont("assets/louis-george-cafe.ttf", 14);

  if (!cloud_font || !louis_george_cafe_font) {
    fprintf(stderr, "SDL font load failed\n");
    return FALSE;
  }

  return TRUE;
}

void destroy_window(void) {
  TTF_CloseFont(louis_george_cafe_font);
  TTF_CloseFont(cloud_font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_Quit();
  SDL_Quit();
}

SDL_Texture *createTextureFromSurface(SDL_Surface *surface, int destroySurface) {
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (destroySurface) SDL_FreeSurface(surface);
  return texture;
}

SDL_Texture *createTextTexture(const char *text, TTF_Font *font, int line_width) {
  SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, text, white, line_width);
  return createTextureFromSurface(surface, TRUE);
}
