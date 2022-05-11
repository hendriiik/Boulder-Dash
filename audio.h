#ifndef AUDIO
#define AUDIO

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "allegro5/allegro_ttf.h"

typedef struct audio{
    ALLEGRO_SAMPLE* anda;
    ALLEGRO_SAMPLE* diamante;
    ALLEGRO_SAMPLE* empurra;
    ALLEGRO_SAMPLE* morreu;
    ALLEGRO_AUDIO_STREAM* musica;
    ALLEGRO_SAMPLE* queda;
    ALLEGRO_SAMPLE* ganhou;
}audio;

audio* separa_sons();

void destroi_sons(audio* sons);



#endif