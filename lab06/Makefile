SOURCES := sim.c computer.c
HEADERS := computer.h
OUTPUT	:= sim.o

.PHONY: compile default clean check report

compile: $(OUTPUT)

$(OUTPUT): $(SOURCES) $(HEADERS)
	@echo -n "Compiling " $@
	@gcc -O0 -std=c99 -g -Wall -Werror -pedantic -Wfatal-errors -o $(OUTPUT) $(SOURCES)
	@echo "\t SUCCESS."

default: compile

clean:
	@echo "Cleaning: $(OUTPUT)"
	@rm -f $(OUTPUT)
	@$(MAKE) -s -C test clean

check:	compile
	@$(MAKE) -s -C test check

report: compile
	@echo -n "Making report... Please wait."
	@$(MAKE) -s -C test check > tests_report.txt
	@echo "\t Completed."
	@echo "Open tests_report.txt to see the complete report"
