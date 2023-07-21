#define PADDLE_COLOR 255, 255, 255, 255
#define PADDLE_SPEED 250
#define PADDLE_SPREAD_ANGLE 88

typedef struct Paddle {
  SDL_Rect rect;
  double x, y;
  int score;
  int dir; // -1 (up), 0 or 1 (down)
} Paddle;

#define BALL_COLOR 0, 255, 255, 255
#define BALL_SPEED 400

typedef struct Ball {
  SDL_Rect rect;
  double x, y, vel_x, vel_y;
} Ball;