cc=clang
incl=include
src=src
out=game
flags=-std=gnu11 -pedantic -pedantic-errors -O3 -I $(incl) `sdl2-config --cflags --libs` -lm -lSDL2_image
srcs=$(src)/main.c $(src)/movement.c
hdrs=$(incl)/main.h $(incl)/movement.h $(incl)/structs.h

all: $(out)

$(out): $(srcs) $(hdrs)
	$(cc) $(flags) $(srcs) -o $(out)

clean:
	rm $(out)

