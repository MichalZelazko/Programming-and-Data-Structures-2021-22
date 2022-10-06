#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* name;
	float balance;
	int accountNumber;
} CustAccount;

void* myBsearch(const void* key, const void* base, size_t numberOfElements,
				size_t size, int (*compare)(const void*, const void*));
int compareStrings(const void* key, const void* element);
int compareIntegers(const void* key, const void* element);
int compareFloats(const void* key, const void* element);
int compareAccountNumber(const void* key, const void* element);
int compareName(const void* key, const void* element);
int compareBalance(const void* key, const void* element);

void testIntegers();
void testFloats();
void testStrings();
void testStructures();

void intTestResult(int* integers, int keyInteger, size_t numberOfElements);
void floatTestResult(float* floats, float keyFloat, size_t numberOfElements);
void stringTestResult();

int main(int argc, char* argv[]) {
	testIntegers();
	testFloats();
	testStrings();
	testStructures();
	return 0;
}


void* myBsearch(const void* key, const void* base, size_t numberOfElements,
				size_t elementSize, int (*compare)(const void*, const void*)) {
	size_t middle;
	size_t beginning = 0;
	size_t end = numberOfElements;
	int compareValue;
	const char* currentElement;
	while (beginning < end) {
		middle = beginning + (end - beginning) / 2;
		currentElement = (const char*)base + (middle * elementSize);
		compareValue = (*compare)(key, currentElement);
		if (compareValue < 0) {
			end = middle;
		} else if (compareValue > 0) {
			beginning = middle + 1;
		} else {
			return (void*)currentElement;
		}
	}
	return NULL;
}

int compareStrings(const void* key, const void* element) {
	char* string1 = *((char**)key);
	char* string2 = *((char**)element);
	return strcmp(string1, string2);
}

int compareIntegers(const void* key, const void* element) {
	return (*(int*)key - *(int*)element);
}

int compareFloats(const void* key, const void* element) {
	float float1 = *(float*)key;
	float float2 = *(float*)element;
	float result = float1 - float2;
	if (result == 0)
		return 0;
	else if (result < 0)
		return -1;
	else
		return 1;
}

int compareAccountNumber(const void* key, const void* element) {
	return ((CustAccount*)key)->accountNumber -
		   ((CustAccount*)element)->accountNumber;
}

int compareName(const void* key, const void* element) {
	return strcmp(((CustAccount*)key)->name, ((CustAccount*)element)->name);
}

int compareBalance(const void* key, const void* element) {
	float float1 = ((CustAccount*)key)->balance;
	float float2 = ((CustAccount*)element)->balance;
	float result = float1 - float2;
	if (result == 0)
		return 0;
	else if (result < 0)
		return -1;
	else
		return 1;
}

void testIntegers() {
	printf("\nINTEGER TESTS\n\n");
	int integers[] = {1, 2, 3, 4, 6};
	int keyInteger = 0;
	int keyInteger2 = 1;
	int keyInteger3 = 2;
	int keyInteger4 = 4;
	int keyInteger5 = 5;
	int keyInteger6 = 3;
	size_t numberOfElements = sizeof(integers) / sizeof(integers[0]);	
	intTestResult(integers, keyInteger, numberOfElements);
	intTestResult(integers, keyInteger2, numberOfElements);
	intTestResult(integers, keyInteger3, numberOfElements);
	intTestResult(integers, keyInteger6, numberOfElements);
	intTestResult(integers, keyInteger4, numberOfElements);
	intTestResult(integers, keyInteger5, numberOfElements);
}

void testFloats() {
	printf("\nFLOAT TESTS\n\n");
	float floats[] = {1.23, 1.43, 8.45};
	float keyFloat = 1.43;
	float keyFloat2 = 1.44;
	float keyFloat3 = 8.45;
	size_t numberOfElements = sizeof(floats) / sizeof(floats[0]);
	floatTestResult(floats, keyFloat, numberOfElements);
	floatTestResult(floats, keyFloat2, numberOfElements);
	floatTestResult(floats, keyFloat3, numberOfElements);
}

void testStrings() {
	printf("\nSTRING TESTS\n\n");
	char* keyString = "abc";
	char* keyString2 = "abd";
	char* keyString3 = "xyz";
	char* strings[] = {"abc", "def", "ght", "kls", "xyz"};
	size_t numberOfElements = sizeof(strings) / sizeof(strings[0]);
	stringTestResult(strings, keyString, numberOfElements);
	stringTestResult(strings, keyString2, numberOfElements);
	stringTestResult(strings, keyString3, numberOfElements);
}

void testStructures() {
	printf("\nSTRUCT TESTS\n\n");
	CustAccount customers[4] = {
		{"A", 0.0, 1}, {"B", 10.0, 2}, {"C", 15.0, 3}, {"D", 20.0, 4}};
	CustAccount keyStruct = {"A", 10.0, 1};
	CustAccount keyStruct2 = {"E", 0.0, 1};
	CustAccount keyStruct3 = {"F", 20.0, 1};
	if (myBsearch(&keyStruct, customers,
				  sizeof(customers) / sizeof(customers[0]),
				  sizeof(customers[0]), compareAccountNumber)) {
		printf("The structure with account number \"%d\" was found\n",
			   keyStruct.accountNumber);
	} else {
		printf("The structure with account number \"%d\" was NOT found\n",
			   keyStruct.accountNumber);
	}
	if (myBsearch(&keyStruct2, customers,
				  sizeof(customers) / sizeof(customers[0]),
				  sizeof(customers[0]), compareName)) {
		printf("The structure with name \"%s\" was found\n", keyStruct2.name);
	} else {
		printf("The structure with name \"%s\" was NOT found\n",
			   keyStruct2.name);
	}
	if (myBsearch(&keyStruct3, customers,
				  sizeof(customers) / sizeof(customers[0]),
				  sizeof(customers[0]), compareBalance)) {
		printf("The structure with balance \"%f\" was found\n",
			   keyStruct3.balance);
	} else {
		printf("The structure with balance \"%f\" was NOT found\n",
			   keyStruct3.balance);
	}
}

void stringTestResult(char** strings, char* keyString, size_t numberOfElements){
	if (myBsearch(&keyString, strings, numberOfElements,
				  sizeof(char*), compareStrings)) {
		printf("The element \"%s\" was found\n", keyString);
	} else {
		printf("The element \"%s\" was NOT found\n", keyString);
	}
}

void floatTestResult(float* floats, float keyFloat, size_t numberOfElements){
	if (myBsearch(&keyFloat, floats, numberOfElements,
				  sizeof(floats[0]), compareFloats)) {
		printf("The element \"%f\" was found\n", keyFloat);
	} else {
		printf("The element \"%f\" was NOT found\n", keyFloat);
	}
}

void intTestResult(int* integers, int keyInteger, size_t numberOfElements){
	if (myBsearch(&keyInteger, integers,
				  numberOfElements, sizeof(integers[0]),
				  compareIntegers)) {
		printf("The element \"%d\" was found\n", keyInteger);
	} else {
		printf("The element \"%d\" was NOT found\n", keyInteger);
	}
}
