SOURCES := array_gen.c count.c timer.c
HEADERS := array_gen.h count.h

all: timer

.PHONY: test

timer: $(SOURCES) $(HEADERS)
	gcc -o timer $(SOURCES) -g -Wall -Werror -Wfatal-errors

test: timer
	@./timer

clean:
	rm -f timer
