#define APP_NAME "C Pong"

#define TRUE 1
#define FALSE 0

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540
#define WINDOW_CLEAR_COLOR 38, 55, 76, 255

#define FPS 60
#define TARGET_FRAME_TIME (1000 / FPS)
#define ENABLE_FRAME_RATE_CAP TRUE

#define PI 3.142857143
#define CLAMP(X, MIN, MAX) (X < MIN ? MIN : X > MAX ? MAX : X)
#define DEG2RAD(X) (X / 360.0f * PI)