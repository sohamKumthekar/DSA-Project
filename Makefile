CC = cc
CFLAGS = -lm -lGL -lglut -lGLU

all: test1

test1: *.c
	$(CC) $^ -o $@ $(CFLAGS)

run: test1
	./test1

clean:
	rm -f test1
