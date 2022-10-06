#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (gfx_init())
    exit(3);
  for (int i = -179; i < 180; ++i) {
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
    gfx_filledCircle(gfx_screenWidth() / 2 + i, gfx_screenHeight() / 2, 100, YELLOW);
    gfx_filledCircle(gfx_screenWidth() / 2 + gfx_screenWidth() / 4 * sin(i*M_PI/180), gfx_screenHeight() / 2 + gfx_screenHeight() / 4 * cos(i*M_PI/180), 50, RED);
    gfx_updateScreen();
    SDL_Delay(10);
  }
  gfx_getkey();
  return 0;
}
