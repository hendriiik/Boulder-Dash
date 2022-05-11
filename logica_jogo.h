#ifndef LOGICA_JOGO
#define LOGICA_JOGO

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "allegro5/allegro_ttf.h"

int checa_vitoria(mapa* m,player *p);

void cheat_debug(mapa *m);

int checa_validade_e_atualiza_diamantes (mapa *m,player *p, int linha, int coluna,audio *sons);

int pode_empurrar_pedra(mapa *m, int linha, int coluna, int direcao);

void checa_pedras_na_cabeca(mapa *m, player *p);

void derruba_pedras(mapa *m,player *p,audio *sons);

void empurra_pedra(mapa *m,player *p, audio *sons,int linha, int coluna, int direcao);

void player_move(mapa *m,player *p,audio *sons,int direcao);

int checa_debug(int debug[5]);

void checa_morte(mapa *m, player *p, audio *s);

void move_monstros(mapa *m,player *p,audio *sons);

int monstro_pode_andar(mapa *m, player *p,int i, int j, int direcao);


#endif

