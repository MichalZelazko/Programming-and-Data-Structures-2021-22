# List your *.h files (if you do not have them in your project then leave the variable "headers" empty):
headers =

# List your *.c files:
sources = bsearch.c

# Specify name of your program:
executable = bsearch

$(executable): $(headers) $(sources)
	gcc -g -Wall -pedantic $(sources) -o $(executable)

.PHONY: clean
clean:
	rm $(executable)

.PHONY: check
check: $(executable)
	valgrind --leak-check=full --track-origins=yes ./$(executable)
