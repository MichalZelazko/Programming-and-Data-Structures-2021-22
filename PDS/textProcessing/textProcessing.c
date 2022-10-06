#include "textProcessing.h"

bool allocFail = false;

int main() {
	char** lines = NULL;
	size_t numberOfLines = 0;
	lines = readInput(lines, &numberOfLines);
	lines = printAllLinesReversed(lines, numberOfLines);
	fullFree(lines, numberOfLines);
	return 0;
}

char* readLine() {
	char* line = NULL;
	int c;
	size_t bufferSize = 0;
	size_t size = 0;
	while ((c = getchar()) != EOF && !allocFail) {
		if (size >= bufferSize) {
			char* newBuffer;
			if (bufferSize == 0)
				bufferSize = 2;
			else if (bufferSize <= ((size_t)-1) / 2)
				bufferSize = 2 * size;
			else {
				lineErrorFree(line);
			}
			newBuffer = realloc(line, bufferSize);
			if (!newBuffer) {
				lineErrorFree(line);
			}
			line = newBuffer;
		}
		line[size++] = c;
		if (c == '\n')
			break;
	}
	if ((c == EOF) && (size == 0))
		return NULL;
	if (size >= bufferSize) {
		char* newBuffer;
		if (size < (size_t)-1)
			bufferSize = size + 1;
		else {
			lineErrorFree(line);
		}
		newBuffer = realloc(line, bufferSize);
		if (!newBuffer) {
			lineErrorFree(line);
		}
		line = newBuffer;
	}
	line[size++] = '\0';
	return line;
}

char** readInput(char** lines, size_t* numberOfLines) {
	size_t maxNumberOfLines = 0;
	char* line;
	while ((line = readLine()) && !allocFail) {
		if (*numberOfLines >= maxNumberOfLines) {
			char** newLines;
			if (*numberOfLines == 0)
				maxNumberOfLines = 1;
			else if (*numberOfLines <= ((size_t)-1) / 2 / sizeof(char*)) {
				maxNumberOfLines = 2 * *numberOfLines;
			} else {
				arrayErrorFree(lines, *numberOfLines);
				break;
			}
			newLines = realloc(lines, maxNumberOfLines * sizeof(char*));
			if (newLines == NULL) {
				arrayErrorFree(lines, *numberOfLines);
				break;
			}
			lines = newLines;
		}
		lines[*numberOfLines] = line;
		*numberOfLines += 1;
	}
	return lines;
}

char** extractWords(size_t* wordsInLine, size_t* maxWordsInLine, char** lines,
					int i) {
	char** words = NULL;
	char* word;
	word = strtok(lines[i], " \n");
	while (word != NULL){
		if (*wordsInLine >= *maxWordsInLine) {
			char** newWords;
			if (*wordsInLine == 0)
				*maxWordsInLine = 1;
			else if (*wordsInLine <= ((size_t)-1) / 2 / sizeof(char*)) {
				*maxWordsInLine = 2 * *wordsInLine;
			} else {
				arrayErrorFree(words, *wordsInLine);
				break;
			}
			newWords = realloc(words, *maxWordsInLine * sizeof(char*));
			if (newWords == NULL) {
				arrayErrorFree(words, *wordsInLine);
			}
			words = newWords;
		}
		words[*wordsInLine] = word;
		*wordsInLine += 1;
		word = strtok(NULL, " \n");
	}
	free(word);
	return words;
}

void printWordsReversed(char** words, size_t* wordsInLine) {
	int i = *wordsInLine - 1;
	while(i >= 0){
		if (words[i] != NULL) {
			printf("%s", words[i]);
			if (i != 0) {
				printf(" ");
			}
		}
		i--;
	}
}

char** printAllLinesReversed(char** lines, size_t numberOfLines) {
	int i = 0;
	printf("\n\nReversed:\n");
	while (i < numberOfLines) {
		char** words = NULL;
		size_t wordsInLine = 0;
		size_t maxWordsInLine = 0;
		words = extractWords(&wordsInLine, &maxWordsInLine, lines, i);
		printWordsReversed(words, &wordsInLine);
		printf("\n");
		free(words);
		i++;
	}
	return lines;
}

void lineErrorFree(char* line) {
	allocFail = true;
	free(line);
}

void arrayErrorFree(char** array, size_t sizeOfArray) {
	allocFail = true;
	for (int i = sizeOfArray; i > 0; i--)
		free(array[i - 1]);
	free(array);
}

void fullFree(char** lines, size_t numberOfLines) {
	for (int i = 0; i < numberOfLines; i++) {
		free(lines[i]);
	}
	free(lines);
}
