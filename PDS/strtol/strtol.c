#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII0 48
#define ASCII9 57
#define ASCIIA 55

bool isBaseCorrect(int base);
int determineSign(const char** pos);
int convertToInt(int number, const char* pos);
int determineBaseWithZeroPrefix(int base, const char** pos, const char** endpos);
int determineBase(int base, const char** pos, const char** endpos);
long int calculateNegativeSum(long int sum, int number, int base);
long int calculatePositiveSum(long int sum, int number, int base);
void skipSpaces(const char** pos);

long strtol(const char* nPtr, char** endPtr, int base) {
	long int sum = 0;
	int number = 0;
	const char* pos = nPtr;
	const char* endpos = nPtr;
	if (*pos == '\0')
		return 0;
	skipSpaces(&pos);
	int sign = determineSign(&pos);
	base = determineBase(base, &pos, &endpos);
	if (!isBaseCorrect(base)) {
		return 0;
	}
	while (*pos != '\0') {
		number = convertToInt(number, pos);
		if (number < base && number != -1) {
			if (sign == -1) {
				sum = calculateNegativeSum(sum, number, base);
			} else {
				sum = calculatePositiveSum(sum, number, base);
			}
		} else if (base == 16 && number > base &&
				   (*(pos - 1) == 'x' || *(pos - 1) == 'X')) {
			pos--;
			break;
		} else
			break;
		endpos = ++pos;
	}
	if (endPtr)
		*endPtr = (char*)endpos;
	return sum;
}

bool isBaseCorrect(int base) {
	if ((base < 2 || base > 36) && base != 0) {
		errno = EINVAL;
		return false;
	} else {
		return true;
	}
}

int convertToInt(int number, const char* pos) {
	number = -1;
	if ((int)*pos >= ASCII0 && (int)*pos <= ASCII9) {
		number = (int)*pos - ASCII0;
	}
	if (isalpha(*pos)) {
		number = (int)toupper(*pos) - ASCIIA;
	}
	return number;
}

int determineBaseWithZeroPrefix(int base, const char** pos, const char** endpos) {
	(*pos)++;
	*endpos = *pos;
	if (base == 16 && (**pos == 'X' || **pos == 'x')) {
		(*pos)++;
	} else if (base == 0) {
		if (**pos == 'X' || **pos == 'x') {
			base = 16;
			(*pos)++;
		} else {
			base = 8;
		}
	}
	return base;
}

int determineBase(int base, const char** pos, const char** endpos){
	if (**pos == '0') {
		base = determineBaseWithZeroPrefix(base, pos, endpos);
	} else if (base == 0) {
		base = 10;
	}
	return base;
}

int determineSign(const char** pos) {
	int sign;
	if (**pos == '-') {
		sign = -1;
		(*pos)++;
	} else if (**pos == '+') {
		sign = 1;
		(*pos)++;
	} else {
		sign = 1;
	}
	return sign;
}

long int calculateNegativeSum(long int sum, int number, int base) {
	if (sum >= ((LONG_MIN + number) / base)) {
		sum = sum * base - number;
	} else {
		errno = ERANGE;
		sum = LONG_MIN;
	}
	return sum;
}

long int calculatePositiveSum(long int sum, int number, int base) {
	if (sum <= ((LONG_MAX - number) / base)) {
		sum = sum * base + number;
	} else {
		errno = ERANGE;
		sum = LONG_MAX;
	}
	return sum;
}

void skipSpaces(const char** pos) {
	while (isspace(**pos)) {
		(*pos)++;
	}
}
