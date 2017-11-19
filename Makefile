cc=gcc
incl=include
src=src
client=game
server=server

bothFlags=-std=gnu11 -pedantic -pedantic-errors -O3 -I $(incl) -Wall -Werror
clFlags=`sdl2-config --cflags --libs` -lm -lSDL2_image
srvFlags=

clSrc=$(src)/client.c $(src)/movement.c $(bothSrc)
clHdrs=$(incl)/main.h $(incl)/movement.h $(incl)/structs.h $(bothHdrs)

bothSrc=$(src)/serialize.c $(src)/network.c 
bothHdrs=$(incl)/serialize.h $(incl)/connect.h $(incl)/network.h 

srvSrc=$(src)/server.c $(bothSrc)
srvHdrs=$(incl)/server.h $(incl)/connect.h $(bothHdrs)

all: $(client) $(server)

$(client): $(clSrc) $(clHdrs)
	$(cc) $(bothFlags) $(clFlags) $(clSrc) -o $(client)

$(server): $(srvSrc) $(srvHdrs)
	$(cc) $(bothFlags) $(srvFlags) $(srvSrc) -o $(server)

clean:
	rm $(client) $(server)

