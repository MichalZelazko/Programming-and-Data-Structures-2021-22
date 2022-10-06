#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readLine();
char** readInput(char** lines, size_t* numberOfLines);
char** extractWords(size_t* wordsInLine, size_t* maxWordsInLine, char** lines,
					int i);
void printWordsReversed(char** words, size_t* wordsInLine);
char** printAllLinesReversed(char** lines, size_t numberOfLines);
void arrayErrorFree(char** lines, size_t numberOfLines);
void lineErrorFree(char* line);
void fullFree(char** lines, size_t numberOfLines);
