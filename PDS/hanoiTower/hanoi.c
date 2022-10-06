#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>

#define ELEMENTS 3
#define NUMBER_OF_POLES 4
#define POLE_WIDTH 1
#define POLE_HEIGHT (ELEMENTS * BLOCK_HEIGHT + 20)
#define BLOCK_WIDTH (0.8 * (500 / (NUMBER_OF_POLES * ELEMENTS)))
#define BLOCK_HEIGHT (NUMBER_OF_POLES * BLOCK_WIDTH)
#define ANIMATION_HEIGHT                                                       \
	(gfx_screenHeight() - (POLE_HEIGHT + 50 + BLOCK_HEIGHT))
#define ANIMATION_STEP 5

void displayBackground() {
	gfx_filledRect(0, 0, gfx_screenWidth(), gfx_screenHeight(), BLACK);
	for (int i = 1; i <= NUMBER_OF_POLES; i++) {
		gfx_filledRect(
			i * gfx_screenWidth() / (NUMBER_OF_POLES + 1) - POLE_WIDTH / 2,
			gfx_screenHeight() - POLE_HEIGHT,
			i * gfx_screenWidth() / (NUMBER_OF_POLES + 1) + POLE_WIDTH / 2,
			gfx_screenHeight(), RED);
	}
}

void drawBlock(int pole, int value, int pos) {
	int x1 =
		pole * gfx_screenWidth() / (NUMBER_OF_POLES + 1) - (value)*BLOCK_WIDTH;
	int y1 = gfx_screenHeight() - ((pos + 1) * BLOCK_HEIGHT);
	int x2 =
		pole * gfx_screenWidth() / (NUMBER_OF_POLES + 1) + (value)*BLOCK_WIDTH;
	int y2 = gfx_screenHeight() - (pos * BLOCK_HEIGHT);
	gfx_filledRect(x1, y1, x2, y2, GREEN);
	gfx_rect(x1, y1, x2, y2, WHITE);
}

void drawCurrentState(int stacks[NUMBER_OF_POLES][ELEMENTS]) {
	for (int i = 0; i < NUMBER_OF_POLES; i++) {
		for (int j = ELEMENTS - 1; j >= 0; j--) {
			if (stacks[i][j] != 0) {
				drawBlock(i + 1, stacks[i][j], j);
			}
		}
	}
}

void display(int stacks[NUMBER_OF_POLES][ELEMENTS]) {
	displayBackground();
	drawCurrentState(stacks);
}

int disksOnPole(int stacks[NUMBER_OF_POLES][ELEMENTS], int pole) {
	int count = 0;
	for (int j = 0; j <= ELEMENTS - 1; j++) {
		if (stacks[pole - 1][j] != 0) {
			count++;
		}
	}
	return count;
}

int getIntInput() {
	int a = gfx_getkey();
	if (a == 27) {
		printf("\nProgram finished by user");
		exit(1);
	} else {
		a = a - '0'; // converting ASCII to its integer value
		while (a < 0 || a > NUMBER_OF_POLES) {
			printf("Invalid input, choose again\n");
			a = gfx_getkey() - '0';
		}
		return a;
	}
}

int moveValueFrom(int stacks[NUMBER_OF_POLES][ELEMENTS], int pole) {
	int value;
	for (int j = 0; j < ELEMENTS; j++) {
		if (stacks[pole - 1][j] == 0 && j != 0) {
			value = stacks[pole - 1][j - 1];
			return value;
		}
	}
	value = stacks[pole - 1][ELEMENTS - 1];
	return value;
}

void removeValue(int stacks[NUMBER_OF_POLES][ELEMENTS], int pole) {
	for (int j = 0; j < ELEMENTS; j++) {
		if (stacks[pole - 1][j] == 0 && j != 0) {
			stacks[pole - 1][j - 1] = 0;
		}
	}
	stacks[pole - 1][ELEMENTS - 1] = 0;
}

void animateUp(int stacks[NUMBER_OF_POLES][ELEMENTS], int value, int xStart,
			   int yStart) {
	int currentY = yStart;
	while (currentY > ANIMATION_HEIGHT + BLOCK_HEIGHT) {
		display(stacks);
		gfx_filledRect(xStart, currentY - BLOCK_HEIGHT,
					   xStart + (value)*2 * BLOCK_WIDTH, currentY, YELLOW);
		gfx_rect(xStart, currentY - BLOCK_HEIGHT,
				 xStart + (value)*2 * BLOCK_WIDTH, currentY, WHITE);
		gfx_updateScreen();
		currentY -= ANIMATION_STEP;
		SDL_Delay(10);
	}
}

void animateSidewaysLeft(int stacks[NUMBER_OF_POLES][ELEMENTS], int value,
						 int currentX, int xEnd) {
	while (currentX > xEnd) {
		display(stacks);
		gfx_filledRect(currentX, ANIMATION_HEIGHT,
					   currentX + 2 * value * BLOCK_WIDTH,
					   ANIMATION_HEIGHT + BLOCK_HEIGHT, YELLOW);
		gfx_rect(currentX, ANIMATION_HEIGHT, currentX + 2 * value * BLOCK_WIDTH,
				 ANIMATION_HEIGHT + BLOCK_HEIGHT, WHITE);
		currentX -= ANIMATION_STEP;
		gfx_updateScreen();
		SDL_Delay(10);
	}
}

void animateSidewaysRight(int stacks[NUMBER_OF_POLES][ELEMENTS], int value,
						  int currentX, int xEnd) {
	while (currentX < xEnd) {
		display(stacks);
		gfx_filledRect(currentX, ANIMATION_HEIGHT,
					   currentX + 2 * value * BLOCK_WIDTH,
					   ANIMATION_HEIGHT + BLOCK_HEIGHT, YELLOW);
		gfx_rect(currentX, ANIMATION_HEIGHT, currentX + 2 * value * BLOCK_WIDTH,
				 ANIMATION_HEIGHT + BLOCK_HEIGHT, WHITE);
		currentX += ANIMATION_STEP;
		gfx_updateScreen();
		SDL_Delay(10);
	}
}

void animateSideways(int stacks[NUMBER_OF_POLES][ELEMENTS], int value,
					 int xStart, int xEnd) {
	int currentX = xStart;
	if (xStart > xEnd) {
		animateSidewaysLeft(stacks, value, currentX, xEnd);
	} else {
		animateSidewaysRight(stacks, value, currentX, xEnd);
	}
}

void animateDown(int stacks[NUMBER_OF_POLES][ELEMENTS], int value, int xEnd,
				 int yEnd) {
	int currentY = ANIMATION_HEIGHT + BLOCK_HEIGHT;
	while (currentY < yEnd) {
		displayBackground();
		drawCurrentState(stacks);
		gfx_filledRect(xEnd, currentY - BLOCK_HEIGHT,
					   xEnd + (value)*2 * BLOCK_WIDTH, currentY, YELLOW);
		gfx_rect(xEnd, currentY - BLOCK_HEIGHT, xEnd + (value)*2 * BLOCK_WIDTH,
				 currentY, WHITE);
		gfx_updateScreen();
		currentY += ANIMATION_STEP;
		SDL_Delay(10);
	}
}

void animateBlock(int stacks[NUMBER_OF_POLES][ELEMENTS], int from, int to,
				  int value) {
	int heightFrom = disksOnPole(stacks, from);
	int heightTo = disksOnPole(stacks, to);
	int x1, x2, y1, y2;
	x1 = from * gfx_screenWidth() / (NUMBER_OF_POLES + 1) - (value)*BLOCK_WIDTH;
	y1 = gfx_screenHeight() - (heightFrom * BLOCK_HEIGHT);
	x2 = to * gfx_screenWidth() / (NUMBER_OF_POLES + 1) - (value)*BLOCK_WIDTH;
	y2 = gfx_screenHeight() - (heightTo * BLOCK_HEIGHT);
	animateUp(stacks, value, x1, y1);
	animateSideways(stacks, value, x1, x2);
	animateDown(stacks, value, x2, y2);
}

void moveValueTo(int stacks[NUMBER_OF_POLES][ELEMENTS], int from, int to,
				 int value) {
	for (int j = 0; j < ELEMENTS; j++) {
		if (stacks[to - 1][j] == 0) {
			if (j == 0) {
				removeValue(stacks, from);
				animateBlock(stacks, from, to, value);
				stacks[to - 1][j] = value;
				break;
			} else if (stacks[to - 1][j - 1] > value) {
				removeValue(stacks, from);
				animateBlock(stacks, from, to, value);
				stacks[to - 1][j] = value;
				break;
			} else {
				break;
			}
		}
	}
}

void moveBlock(int stacks[NUMBER_OF_POLES][ELEMENTS]) {
	int from = getIntInput();
	int to = getIntInput();
	int value;
	if ((from >= 0 && from <= NUMBER_OF_POLES) &&
		(to >= 0 && to <= NUMBER_OF_POLES) && (from != to)) {
		if (from == 0) {
			from = 10;
		}
		if (to == 0) {
			to = 10;
		}
		value = moveValueFrom(stacks, from);
		if (value != 0) {
			moveValueTo(stacks, from, to, value);
		}
	}
}

void initTowers(int stacks[NUMBER_OF_POLES][ELEMENTS]) {
	for (int i = 0; i < NUMBER_OF_POLES; i++) {
		for (int j = 0; j < ELEMENTS; j++) {
			if (i == 0) {
				stacks[i][j] = ELEMENTS - j;
			} else {
				stacks[i][j] = 0;
			}
		}
	}
}

void endscreen() {
	gfx_filledRect(0, 0, gfx_screenWidth(), gfx_screenHeight(), BLACK);
	gfx_textout(gfx_screenWidth() / 3, gfx_screenHeight() / 2,
				"Congratulations! Press anything to escape", YELLOW);
	gfx_updateScreen();
}

int main(int argc, char* argv[]) {
	if (gfx_init())
		exit(3);
	int stacks[NUMBER_OF_POLES][ELEMENTS];
	initTowers(stacks);
	display(stacks);
	gfx_updateScreen();
	while (stacks[NUMBER_OF_POLES - 1][ELEMENTS - 1] != 1) {
		moveBlock(stacks);
		display(stacks);
		gfx_updateScreen();
	}
	SDL_Delay(500);
	endscreen();
	gfx_getkey();
	return 0;
}
