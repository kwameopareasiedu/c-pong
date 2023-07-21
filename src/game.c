#include "objects.c"

static int game_running = FALSE;

static Paddle paddle_l, paddle_r;
static Ball ball;

static SDL_Texture *title_text_tex, *score_tex, *credits_tex;
static SDL_Rect title_tex_rect = {10, 10, 0, 0},
    score_rect = {WINDOW_WIDTH - 10, 10, 0, 0},
    credits_rect = {WINDOW_WIDTH - 10, 0, 0, 0};

void setup() {
  // Setup left paddle dimensions
  paddle_l.x = 20;
  paddle_l.y = 0.5 * WINDOW_HEIGHT - 50;
  paddle_l.rect.x = (int) paddle_l.x;
  paddle_l.rect.y = (int) paddle_l.y;
  paddle_l.rect.w = 10;
  paddle_l.rect.h = 100;
  paddle_l.dir = 0;
  paddle_l.score = 0;

  // Setup right paddle dimensions
  paddle_r.x = WINDOW_WIDTH - 40;
  paddle_r.y = 0.5 * WINDOW_HEIGHT - 50;
  paddle_r.rect.x = (int) paddle_r.x;
  paddle_r.rect.y = (int) paddle_r.y;
  paddle_r.rect.w = 10;
  paddle_r.rect.h = 100;
  paddle_r.dir = 0;
  paddle_r.score = 0;

  // Setup ball dimensions
  ball.x = 0.5 * WINDOW_WIDTH - 5;
  ball.y = 0.5 * WINDOW_HEIGHT - 5;
  ball.rect.x = (int) ball.x;
  ball.rect.y = (int) ball.y;
  ball.rect.w = 10;
  ball.rect.h = 10;
  ball.vel_x = -BALL_SPEED * cos(0);

  // Setup static text textures
  char *title_text = "Pong!";
  TTF_SizeText(cloud_font, title_text, &title_tex_rect.w, &title_tex_rect.h);
  title_text_tex = createTextTexture(title_text, cloud_font, 1000);

  char *credits_text = "Created By   Kwame Opare Asiedu   in   C";
  TTF_SizeText(louis_george_cafe_font, credits_text, &credits_rect.w, &credits_rect.h);
  credits_tex = createTextTexture(credits_text, louis_george_cafe_font, 2048);
  credits_rect.x = WINDOW_WIDTH / 2 - credits_rect.w / 2;
  credits_rect.y = WINDOW_HEIGHT - credits_rect.h / 2 - 20;
}

void process_input() {
  SDL_Event ev;
  SDL_PollEvent(&ev);

  switch (ev.type) {
    case SDL_QUIT:
      game_running = FALSE;
      break;
    case SDL_KEYDOWN: {
      int auto sym = ev.key.keysym.sym;
      if (sym == SDLK_w) paddle_l.dir = -1;
      if (sym == SDLK_s) paddle_l.dir = 1;
      if (sym == SDLK_UP) paddle_r.dir = -1;
      if (sym == SDLK_DOWN) paddle_r.dir = 1;
      break;
    }
    case SDL_KEYUP: {
      int auto sym = ev.key.keysym.sym;
      if (sym == SDLK_w || sym == SDLK_s) paddle_l.dir = 0;
      if (sym == SDLK_UP || sym == SDLK_DOWN) paddle_r.dir = 0;
      break;
    }
  }
}

void update(double dt) {
  // Move left paddle
  if (paddle_l.dir) {
    paddle_l.y += paddle_l.dir * PADDLE_SPEED * dt;
    paddle_l.y = CLAMP(paddle_l.y, 0, WINDOW_HEIGHT - paddle_l.rect.h);
    paddle_l.rect.y = (int) paddle_l.y;
  }

  // Move right paddle
  if (paddle_r.dir) {
    paddle_r.y += paddle_r.dir * PADDLE_SPEED * dt;
    paddle_r.y = CLAMP(paddle_r.y, 0, WINDOW_HEIGHT - paddle_r.rect.h);
    paddle_r.rect.y = (int) paddle_r.y;
  }

  // Check for collision
  SDL_Rect col;

  if (SDL_IntersectRect(&paddle_l.rect, &ball.rect, &col)) {
    double rel_y = (paddle_l.y + 0.5 * paddle_l.rect.h) - (col.y + 0.5 * col.h);
    double bounce_angle_deg =
        rel_y / (0.5 * paddle_l.rect.h) * PADDLE_SPREAD_ANGLE;

    ball.vel_x = BALL_SPEED * cos(DEG2RAD(bounce_angle_deg));
    ball.vel_y = -BALL_SPEED * sin(DEG2RAD(bounce_angle_deg));
  } else if (SDL_IntersectRect(&paddle_r.rect, &ball.rect, &col)) {
    double rel_y = (paddle_r.y + 0.5 * paddle_l.rect.h) - (col.y + 0.5 * col.h);
    double bounce_angle_deg =
        rel_y / (0.5 * paddle_r.rect.h) * PADDLE_SPREAD_ANGLE;

    ball.vel_x = -BALL_SPEED * cos(DEG2RAD(bounce_angle_deg));
    ball.vel_y = -BALL_SPEED * sin(DEG2RAD(bounce_angle_deg));
  }

  // Reverse the ball vel_y when it hits the top or bottom of window
  if (ball.y < 0 || ball.y + ball.rect.h > WINDOW_HEIGHT) ball.vel_y *= -1;

  // Update ball position based on its velocity
  if (ball.vel_x || ball.vel_y) {
    ball.x += ball.vel_x * dt;
    ball.y += ball.vel_y * dt;
    ball.rect.x = (int) ball.x;
    ball.rect.y = (int) ball.y;
  }

  if (ball.x <= 0 || ball.x + ball.rect.w >= WINDOW_WIDTH) {
    int next_dir;

    if (ball.x <= 0) {
      paddle_r.score++;
      next_dir = 1;
    } else {
      paddle_l.score++;
      next_dir = -1;
    }

    // Reset ball position
    ball.x = 0.5 * WINDOW_WIDTH - 5;
    ball.y = 0.5 * WINDOW_HEIGHT - 5;
    ball.vel_x = next_dir * BALL_SPEED * cos(0);
    ball.vel_y = 0;

    // Reset paddle Y positions
    paddle_l.y = paddle_r.y = 0.5 * WINDOW_HEIGHT - 0.5 * paddle_l.rect.h;
    paddle_l.rect.y = (int) paddle_l.y;
    paddle_r.rect.y = (int) paddle_r.y;
  }
}

void render() {
  SDL_SetRenderDrawColor(renderer, WINDOW_CLEAR_COLOR);
  SDL_RenderClear(renderer);

  // Draw paddles
  SDL_SetRenderDrawColor(renderer, PADDLE_COLOR);
  SDL_RenderFillRect(renderer, &paddle_l.rect);
  SDL_SetRenderDrawColor(renderer, PADDLE_COLOR);
  SDL_RenderFillRect(renderer, &paddle_r.rect);

  // Draw ball
  SDL_SetRenderDrawColor(renderer, BALL_COLOR);
  SDL_RenderFillRect(renderer, &ball.rect);

  // Draw middle line
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 92);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT);

  // Draw text
  SDL_RenderCopy(renderer, title_text_tex, NULL, &title_tex_rect);
  SDL_RenderCopy(renderer, credits_tex, NULL, &credits_rect);

  // Draw score
  char score_text[16];
  sprintf(score_text, "%d : %d", paddle_l.score, paddle_r.score);
  TTF_SizeText(cloud_font, score_text, &score_rect.w, &score_rect.h);
  score_rect.x = (WINDOW_WIDTH - score_rect.w - 10);
  score_tex = createTextTexture(score_text, cloud_font, 200);

  SDL_RenderCopy(renderer, score_tex, NULL, &score_rect);

  SDL_RenderPresent(renderer);
}