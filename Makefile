ALLEGRO = $$(pkg-config --libs allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags)

all: jogo

jogo: main.o audio.o boulder.o logica_jogo.o sprites.o
	gcc -Wall -o  boulder_dash main.c audio.c boulder.c logica_jogo.c sprites.c $(ALLEGRO)

main.o: main.c audio.h boulder.h logica_jogo.h sprites.h
audio.o: audio.c audio.h
boulder.o: boulder.c boulder.h
logica_jogo.o: logica_jogo.c logica_jogo.h
sprites.o: sprites.c sprites.h

clean:
	-rm -f *~ *.o

purge:
	-rm -f *.o boulder_dash
