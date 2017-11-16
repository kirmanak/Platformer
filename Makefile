cc=gcc
incl=include
src=src
out=game
flags=-std=gnu11 -pedantic -pedantic-errors -O0 -I $(incl) `sdl2-config --cflags --libs` -lm -lSDL2_image

all: $(out)

$(out): $(src)/main.c
	$(cc) $(flags) $(src)/main.c -o $(out)

clean:
	rm $(out)

