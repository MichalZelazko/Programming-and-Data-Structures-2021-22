# List your *.h files (if you do not have them in your project then leave the variable "headers" empty):
headers = textProcessing.h

# List your *.c files:
sources = textProcessing.c

# Specify name of your program:
executable = textProcessing

$(executable): $(headers) $(sources)
	gcc -g -Wall -pedantic $(sources) -o $(executable)

.PHONY: clean
clean:
	rm $(executable)

.PHONY: check
check: $(executable)
	valgrind --leak-check=full --track-origins=yes ./$(executable)
