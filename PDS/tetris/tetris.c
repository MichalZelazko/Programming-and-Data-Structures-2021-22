#include "primlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define GAME_WIDTH 10
#define GAME_HEIGHT 20
#define TOP_LEFT_X (gfx_screenWidth() / 2 - GAME_WIDTH / 2 * SQUARE_SIZE)
#define TOP_LEFT_Y (gfx_screenHeight() - GAME_HEIGHT * SQUARE_SIZE)
#define SQUARE_SIZE ((gfx_screenHeight() - 100) / GAME_HEIGHT)
#define NEXT_PIECE_GAP 20
#define END_DISPLAY_TIME 2000
#define TEXT_X (gfx_screenWidth() / 2 - 20)
#define TEXT_Y (gfx_screenHeight() / 2)
#define ASCII_ENTER 13
#define TIME_FOR_MOVE 5
#define POSSIBLE_PIECES 7
#define POSSIBLE_ROTATIONS 4

char pieces[7 /*kind */][4 /* rotation */][5][5] = {
	/* square */
	{{{0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0},
	  {0, 0, 2, 1, 0},
	  {0, 0, 1, 1, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0},
	  {0, 0, 2, 1, 0},
	  {0, 0, 1, 1, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0},
	  {0, 0, 2, 1, 0},
	  {0, 0, 1, 1, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0},
	  {0, 0, 2, 1, 0},
	  {0, 0, 1, 1, 0},
	  {0, 0, 0, 0, 0}}},
	/* I */
	{{{0, 0, 0, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 2, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 1, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0},
	  {1, 1, 2, 1, 0},
	  {0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 1, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 2, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0},
	  {0, 1, 2, 1, 1},
	  {0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0}}},
	/* L */
	{{{0, 0, 0, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 2, 0, 0},
	  {0, 0, 1, 1, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0},
	  {0, 1, 2, 1, 0},
	  {0, 1, 0, 0, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 1, 1, 0, 0},
	  {0, 0, 2, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 0, 1, 0},
	  {0, 1, 2, 1, 0},
	  {0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0}}},
	/* L mirrored */
	{{{0, 0, 0, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 2, 0, 0},
	  {0, 1, 1, 0, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 1, 0, 0, 0},
	  {0, 1, 2, 1, 0},
	  {0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 1, 1, 0},
	  {0, 0, 2, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0},
	  {0, 1, 2, 1, 0},
	  {0, 0, 0, 1, 0},
	  {0, 0, 0, 0, 0}}},
	/* N */
	{{{0, 0, 0, 0, 0},
	  {0, 0, 0, 1, 0},
	  {0, 0, 2, 1, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0},
	  {0, 1, 2, 0, 0},
	  {0, 0, 1, 1, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 1, 2, 0, 0},
	  {0, 1, 0, 0, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 1, 1, 0, 0},
	  {0, 0, 2, 1, 0},
	  {0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0}}},
	/* N mirrored */
	{{{0, 0, 0, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 2, 1, 0},
	  {0, 0, 0, 1, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0},
	  {0, 0, 2, 1, 0},
	  {0, 1, 1, 0, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 1, 0, 0, 0},
	  {0, 1, 2, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 1, 1, 0},
	  {0, 1, 2, 0, 0},
	  {0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0}}},
	/* T */
	{{{0, 0, 0, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 2, 1, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0},
	  {0, 1, 2, 1, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 1, 2, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 0, 0, 0, 0}},
	 {{0, 0, 0, 0, 0},
	  {0, 0, 1, 0, 0},
	  {0, 1, 2, 1, 0},
	  {0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0}}}};

int area[GAME_HEIGHT][GAME_WIDTH];
bool end = false;
int nextPieceIndex;
int nextPieceRotation;

void initData();
void drawBoundaries();
void drawArea();
void drawNextPiece();
void drawGame(char nowPiece[5][5], int positionX, int positionY);
bool collides(char nowPiece[5][5], int positionX, int positionY);
bool isLost();
void pieceToArea(char nowPiece[5][5], int positionX, int positionY);
int rowFull();
void emptyRow(int index);
void swapRows(int index);
void deleteFullRows();
void fall(char nowPiece[5][5], int positionX, int positionY);
void play(char nowPiece[4][5][5], int nowPieceRotation);
void endscreen();

int main() {
	if (gfx_init())
		exit(3);
	int nowPiece;
	int nowPieceRotation;
	srand(time(NULL));
	nowPiece = (int)rand() % POSSIBLE_PIECES;
	nowPieceRotation = (int)rand() % POSSIBLE_ROTATIONS;
	nextPieceIndex = (int)rand() % POSSIBLE_PIECES;
	nextPieceRotation = (int)rand() % POSSIBLE_ROTATIONS;
	initData();
	while (end == 0) {
		play(pieces[nowPiece], nowPieceRotation);
		nowPiece = nextPieceIndex;
		nowPieceRotation = nextPieceRotation;
		nextPieceIndex = (int)rand() % POSSIBLE_PIECES;
		nextPieceRotation = (int)rand() % POSSIBLE_ROTATIONS;
	}
	endscreen();
	return 0;
}

void initData() {
	for (int i = 0; i < GAME_HEIGHT; i++) {
		for (int j = 0; j < GAME_WIDTH; j++) {
			area[i][j] = 0;
		}
	}
}

void drawBoundaries() {
	gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
	gfx_line(TOP_LEFT_X - 1, TOP_LEFT_Y - SQUARE_SIZE, TOP_LEFT_X - 1,
			 TOP_LEFT_Y + GAME_HEIGHT * SQUARE_SIZE, YELLOW);
	gfx_line(TOP_LEFT_X + SQUARE_SIZE * GAME_WIDTH + 1,
			 TOP_LEFT_Y - SQUARE_SIZE,
			 TOP_LEFT_X + SQUARE_SIZE * GAME_WIDTH + 1,
			 TOP_LEFT_Y + GAME_HEIGHT * SQUARE_SIZE, YELLOW);
	gfx_line(TOP_LEFT_X, TOP_LEFT_Y, TOP_LEFT_X + SQUARE_SIZE * GAME_WIDTH,
			 TOP_LEFT_Y, YELLOW);
}

void drawArea() {
	drawBoundaries();
	for (int i = 0; i < GAME_HEIGHT; i++) {
		for (int j = 0; j < GAME_WIDTH; j++) {
			if (area[i][j] != 0) {
				int x1 = TOP_LEFT_X + j * SQUARE_SIZE;
				int y1 = TOP_LEFT_Y + i * SQUARE_SIZE;
				int x2 = x1 + SQUARE_SIZE;
				int y2 = y1 + SQUARE_SIZE;
				gfx_filledRect(x1, y1, x2, y2, RED);
			}
		}
	}
}

void drawNextPiece() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (pieces[nextPieceIndex][nextPieceRotation][i][j] != 0) {
				int x1 = TOP_LEFT_X + SQUARE_SIZE * GAME_WIDTH +
						 NEXT_PIECE_GAP + j * SQUARE_SIZE;
				int y1 = TOP_LEFT_Y + i * SQUARE_SIZE;
				int x2 = x1 + SQUARE_SIZE;
				int y2 = y1 + SQUARE_SIZE;
				if (pieces[nextPieceIndex][nextPieceRotation][i][j] == 2) {
					gfx_filledRect(x1, y1, x2, y2, YELLOW);
				} else {
					gfx_filledRect(x1, y1, x2, y2, GREEN);
				}
			}
		}
	}
}

void drawGame(char nowPiece[5][5], int positionX, int positionY) {
	drawArea();
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (nowPiece[i][j] != 0) {
				int x1 = TOP_LEFT_X + (j + positionX) * SQUARE_SIZE;
				int y1 = TOP_LEFT_Y + (i + positionY) * SQUARE_SIZE;
				int x2 = x1 + SQUARE_SIZE;
				int y2 = y1 + SQUARE_SIZE;
				if (nowPiece[i][j] == 2) {
					gfx_filledRect(x1, y1, x2, y2, YELLOW);
				} else {
					gfx_filledRect(x1, y1, x2, y2, GREEN);
				}
			}
		}
	}
	drawNextPiece();
	gfx_updateScreen();
}

bool collides(char nowPiece[5][5], int positionX, int positionY) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (positionX + j >= GAME_WIDTH && nowPiece[i][j] != 0)
				return true;
			if (positionY + i >= GAME_HEIGHT && nowPiece[i][j] != 0)
				return true;
			if (positionX + j < 0 && nowPiece[i][j] != 0)
				return true;
			else if (nowPiece[i][j] != 0 &&
					 area[positionY + i][positionX + j] != 0)
				return true;
		}
	}
	return false;
}

bool isLost() {
	bool isLost = false;
	for (int j = 0; j < GAME_WIDTH; j++) {
		if (area[0][j] != 0) {
			isLost = true;
		}
	}
	return isLost;
}

void pieceToArea(char nowPiece[5][5], int positionX, int positionY) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (nowPiece[i][j] == 2) {
				area[positionY + i][positionX + j] = 1;
			} else if (nowPiece[i][j] == 1) {
				area[positionY + i][positionX + j] = nowPiece[i][j];
			}
		}
	}
	if (isLost()) {
		end = true;
	}
	drawArea();
	gfx_updateScreen();
}

int rowFull() {
	int index = 0;
	for (int i = 0; i < GAME_HEIGHT; i++) {
		bool isFull = true;
		for (int j = 0; j < GAME_WIDTH; j++) {
			if (area[i][j] == 0)
				isFull = false;
		}
		if (isFull) {
			index = i;
			break;
		}
	}
	return index;
}

void emptyRow(int index) {
	for (int j = 0; j < GAME_WIDTH; j++) {
		area[index][j] = 0;
	}
}

void swapRows(int index) {
	for (int i = index - 1; i >= 0; i--) {
		for (int j = 0; j < GAME_WIDTH; j++) {
			int temp = area[i + 1][j];
			area[i + 1][j] = area[i][j];
			area[i][j] = temp;
		}
	}
}

void deleteFullRows() {
	SDL_Delay(100);
	while (rowFull()) {
		int index = rowFull();
		emptyRow(index);
		swapRows(index);
	}
}

void fall(char nowPiece[5][5], int positionX, int positionY) {
	int i = positionY + 1;
	while (!collides(nowPiece, positionX, i)) {
		i++;
	}
	pieceToArea(nowPiece, positionX, i - 1);
}

void play(char nowPiece[4][5][5], int nowPieceRotation) {
	int positionX = GAME_WIDTH / 2 - 2;
	int positionY = 0;
	bool isFallen = false;
	if (collides(nowPiece[nowPieceRotation], positionX, positionY)) {
		SDL_Delay(250);
		end = true;
	}
	while (!collides(nowPiece[nowPieceRotation], positionX, positionY) &&
		   !isFallen && !end) {
		drawGame(nowPiece[nowPieceRotation], positionX, positionY);
		for (int i = 0; i < 100; i++) {
			int pressedKey = gfx_pollkey();
			if (pressedKey == SDLK_DOWN) {
				fall(nowPiece[nowPieceRotation], positionX, positionY);
				isFallen = true;
				break;
			}
			if (pressedKey == SDLK_LEFT) {
				if (!collides(nowPiece[nowPieceRotation], positionX - 1,
							 positionY)) {
					positionX--;
				}
			}
			if (pressedKey == SDLK_RIGHT) {
				if (!collides(nowPiece[nowPieceRotation], positionX + 1,
							 positionY)) {
					positionX++;
				}
			}
			if (pressedKey == SDLK_SPACE) {
				if (nowPieceRotation == 3) {
					if (!collides(nowPiece[0], positionX, positionY)) {
						nowPieceRotation = 0;
					}
				} else {
					if (!collides(nowPiece[nowPieceRotation + 1], positionX,
								 positionY)) {
						nowPieceRotation++;
					}
				}
			}
			if (pressedKey == ASCII_ENTER || pressedKey == SDLK_ESCAPE) {
				end = true;
				break;
			}
			drawGame(nowPiece[nowPieceRotation], positionX, positionY);
			SDL_Delay(TIME_FOR_MOVE);
		}
		positionY++;
		if (collides(nowPiece[nowPieceRotation], positionX, positionY) &&
			!isFallen) {
			pieceToArea(nowPiece[nowPieceRotation], positionX, positionY - 1);
		}
		deleteFullRows();
	}
}

void endscreen() {
	gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
	gfx_textout(TEXT_X, TEXT_Y, "GAME OVER!", YELLOW);
	gfx_updateScreen();
	SDL_Delay(END_DISPLAY_TIME);
}
