SRCS := main.o emulator.o log.o os/linux.o
OBJS := $(filter %.o, $(SRCS))

all: clean chip8_emulator

chip8_emulator: $(OBJS)
	g++ -o $@ $?

%.o: %.cc
	g++ -c $? -o $@ -I.

clean:
	rm -rf chip8_emulator 
	rm -rf $(OBJS)

.PHONY: clean