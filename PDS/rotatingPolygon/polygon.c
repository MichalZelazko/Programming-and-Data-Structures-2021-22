#include "primlib.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMBER_OF_VERTICES 3
#define ANGLE (2 * M_PI / NUMBER_OF_VERTICES)
#define MID_X (gfx_screenWidth() / 2)
#define MID_Y (gfx_screenHeight() / 2)
#define RADIUS (gfx_screenHeight() * 0.9 / 2)
#define ANGLE_STEP 1
#define SCALE_STEP 0.01
#define PERIOD 360

void animatedPolygon(double fi, double scale) {
	for (int vertex = 0; vertex < NUMBER_OF_VERTICES; vertex++) {
		double startPointAngle = vertex * ANGLE + fi;
		double endPointAngle = startPointAngle + ANGLE;
		gfx_line(MID_X + RADIUS * sin(startPointAngle) * scale,
				 MID_Y + RADIUS * cos(startPointAngle) * scale,
				 MID_X + RADIUS * sin(endPointAngle) * scale,
				 MID_Y + RADIUS * cos(endPointAngle) * scale,
				 YELLOW);
	}
}

void display(double fi, double scale) {
	gfx_filledRect(0, 0, gfx_screenWidth(), gfx_screenHeight(), BLACK);
	animatedPolygon(fi, scale);
	gfx_textout(gfx_screenWidth() / 10, gfx_screenHeight() / 10,
				"Press ESCAPE to exit", CYAN);
}

double convertToRadians(int degree) { return degree * M_PI / 180; }

int directionChange(int direction, double scale){
	if (scale >= 1) {
		direction = -1;
	} else if (scale <= 0.1) {
		direction = 1;
	}
	return direction;
}

int main(int argc, char* argv[]) {
	if (gfx_init()) {
		exit(3);
	}
	int direction = 1;
	double angleInRadians = 0;
	double scale = 1.0;
	int angleInDegrees = 0;
	while (!gfx_isKeyDown(SDLK_ESCAPE)) {
		angleInRadians = convertToRadians(angleInDegrees);
		display(angleInRadians, scale);
		angleInDegrees += ANGLE_STEP;
		while (angleInDegrees >= PERIOD) {
			angleInDegrees -= PERIOD;
		}
		scale += SCALE_STEP * direction;
		direction = directionChange(direction, scale);
		gfx_updateScreen();
		SDL_Delay(15);
	}
	return 0;
}
