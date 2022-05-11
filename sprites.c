#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "allegro5/allegro_ttf.h"
#include "sprites.h"
#include "boulder.h"

sprites* separa_sprites(){
    sprites* s;
    ALLEGRO_BITMAP* sheet;

    sheet = al_load_bitmap("resources/spritesheet.jpg");
    must_init(sheet, "spritesheet");

    s=malloc(sizeof(sprites));

    s->personagem[0] = al_create_sub_bitmap(sheet,0,0,16,16);
    s->personagem[1] = al_create_sub_bitmap(sheet,16,0,16,16);
    s->personagem[2] = al_create_sub_bitmap(sheet,32,0,16,16);
    s->personagem[3] = al_create_sub_bitmap(sheet,48,0,16,16);
    s->personagem[4] = al_create_sub_bitmap(sheet,64,0,16,16);
    s->personagem[5] = al_create_sub_bitmap(sheet,80,0,16,16);
    s->personagem[6] = al_create_sub_bitmap(sheet,96,0,16,16);

    s->personagem_e[0] = al_create_sub_bitmap(sheet,0,16,16,16);
    s->personagem_e[1] = al_create_sub_bitmap(sheet,16,16,16,16);
    s->personagem_e[2] = al_create_sub_bitmap(sheet,32,16,16,16);
    s->personagem_e[3] = al_create_sub_bitmap(sheet,48,16,16,16);
    s->personagem_e[4] = al_create_sub_bitmap(sheet,64,16,16,16);
    s->personagem_e[5] = al_create_sub_bitmap(sheet,80,16,16,16);
    s->personagem_e[6] = al_create_sub_bitmap(sheet,96,16,16,16);

    s->personagem_d[0] = al_create_sub_bitmap(sheet,0,32,16,16);
    s->personagem_d[1] = al_create_sub_bitmap(sheet,16,32,16,16);
    s->personagem_d[2] = al_create_sub_bitmap(sheet,32,32,16,16);
    s->personagem_d[3] = al_create_sub_bitmap(sheet,48,32,16,16);
    s->personagem_d[4] = al_create_sub_bitmap(sheet,64,32,16,16);
    s->personagem_d[5] = al_create_sub_bitmap(sheet,80,32,16,16);
    s->personagem_d[6] = al_create_sub_bitmap(sheet,96,32,16,16);

    s->borda = al_create_sub_bitmap(sheet,0,48,16,16);
    s->saida = al_create_sub_bitmap(sheet,16,48,16,16);

    s->terra = al_create_sub_bitmap(sheet,48,48,16,16);
    //s->terra2 = sprites_grab(sheet,64,48,16,16);

    s->muro = al_create_sub_bitmap(sheet,32, 48, 16, 16);
    s->pedra = al_create_sub_bitmap(sheet,80,48,16,16);
    s->tunel = al_create_sub_bitmap(sheet,96,48,16,16);

    s->diamante[0] = al_create_sub_bitmap(sheet,0,64 ,16,16);
    s->diamante[1] = al_create_sub_bitmap(sheet,0,80 ,16,16);
    s->diamante[2] = al_create_sub_bitmap(sheet,0,96 ,16,16);
    s->diamante[3] = al_create_sub_bitmap(sheet,0,112,16,16);
    s->diamante[4] = al_create_sub_bitmap(sheet,16,64,16,16);
    s->diamante[5] = al_create_sub_bitmap(sheet,16,80,16,16);
    s->diamante[6] = al_create_sub_bitmap(sheet,16,96,16,16);
    s->diamante[7] = al_create_sub_bitmap(sheet,16,112,16,16);

    s->amoeba[0] = al_create_sub_bitmap(sheet,64,64,16,16);
    s->amoeba[1] = al_create_sub_bitmap(sheet,64,80,16,16);
    s->amoeba[2] = al_create_sub_bitmap(sheet,64,96,16,16);
    s->amoeba[3] = al_create_sub_bitmap(sheet,64,112,16,16);

    s->vagalume[0] = al_create_sub_bitmap(sheet,80,64,16,16);
    s->vagalume[1] = al_create_sub_bitmap(sheet,80,80,16,16);
    s->vagalume[2] = al_create_sub_bitmap(sheet,80,96,16,16);
    s->vagalume[3] = al_create_sub_bitmap(sheet,80,112,16,16);

    s->borboleta[0] = al_create_sub_bitmap(sheet,96,64,16,16);
    s->borboleta[1] = al_create_sub_bitmap(sheet,96,80,16,16);
    s->borboleta[2] = al_create_sub_bitmap(sheet,96,96,16,16);
    s->borboleta[3] = al_create_sub_bitmap(sheet,96,112,16,16);

    s->muro_magico[0] = al_create_sub_bitmap(sheet,36,64,16,16);
    s->muro_magico[1] = al_create_sub_bitmap(sheet,36,80,16,16);
    s->muro_magico[2] = al_create_sub_bitmap(sheet,36,96,16,16);
    s->muro_magico[3] = al_create_sub_bitmap(sheet,36,112,16,16);



    return s;
}

void destroi_sprites(sprites* s){
    int i;

    for(i=0;i<=6;i++){
        al_destroy_bitmap(s->personagem[i]);
        al_destroy_bitmap(s->personagem_e[i]);
        al_destroy_bitmap(s->personagem_d[i]);
    }

    for(i=0;i<=7;i++){
        al_destroy_bitmap(s->diamante[i]);
    }


    for(i=0;i<=3;i++){
        al_destroy_bitmap(s->amoeba[i]);
        al_destroy_bitmap(s->vagalume[i]);
        al_destroy_bitmap(s->borboleta[i]);
        al_destroy_bitmap(s->muro_magico[i]);
    }



    al_destroy_bitmap(s->borda);
    al_destroy_bitmap(s->saida);
    al_destroy_bitmap(s->terra);
    al_destroy_bitmap(s->muro);
    al_destroy_bitmap(s->pedra);
    al_destroy_bitmap(s->tunel);
}