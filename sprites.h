#ifndef SPRITES
#define SPRITES

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "allegro5/allegro_ttf.h"

#define TAMANHO_SPRITE 16

typedef struct sprites{
    ALLEGRO_BITMAP* personagem[7];
    ALLEGRO_BITMAP* personagem_e[7];
    ALLEGRO_BITMAP* personagem_d[7];
    ALLEGRO_BITMAP* diamante[8];

    ALLEGRO_BITMAP* borda;
    ALLEGRO_BITMAP* saida;
    ALLEGRO_BITMAP* terra;
    ALLEGRO_BITMAP* muro;
    ALLEGRO_BITMAP* pedra;
    ALLEGRO_BITMAP* tunel;

    ALLEGRO_BITMAP* amoeba[4];
    ALLEGRO_BITMAP* borboleta[4];
    ALLEGRO_BITMAP* vagalume[4];
    ALLEGRO_BITMAP* muro_magico[4];
}sprites;

sprites* separa_sprites();

void destroi_sprites(sprites* s);

#endif