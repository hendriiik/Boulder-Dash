#ifndef BOULDER
#define BOULDER

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "allegro5/allegro_ttf.h"
#include "audio.h"
#include "sprites.h"


#define ALTURA 368
#define LARGURA 640
#define DISP_SCALE 2

#define DISP_A (ALTURA* DISP_SCALE)
#define DISP_L (LARGURA* DISP_SCALE)

#define TUNEL 0
#define TERRA 1
#define MURO 2
#define PEDRA 3
#define DIAMANTE 4
#define AMOEBA 5
#define VAGALUME 6
#define BORBOLETA 7
#define PLAYER 8
#define BORDA 9
#define MURO_MAGICO 10
#define SAIDA 11

#define SOBE 1
#define DESCE 2
#define ESQ 3
#define DIR 4

#define FECHOU 0
#define JOGANDO 1
#define ABERTURA 2


typedef struct mapa{
    int linhas;
    int colunas;
    int **matriz;
    int meta_diam;
    int pontos_por_diam;
    int tempo;
    long frames;
    int linha_player;
    int coluna_player;
    int linha_saida;
    int coluna_saida;
    int saida_aberta;
} mapa;

typedef struct player{
    int linha_player;
    int coluna_player;
    int pontuacao;
    int diam;
    int pontos;
    int sprite_parado;
    int sprite_movimento;
    int vivo;
    int direcao;
    int parado;
    int embaixo_pedra;
}player;

void must_init(bool test, const char *description);

void disp_init();

void disp_pre_draw();

void disp_post_draw();

void inicia_tudo();

void le_mapa_e_cria_structs(mapa *m, player *p,int atual);

void imprime_ganhou(mapa* m,player *p, audio *s,ALLEGRO_FONT *font);

int checa_vitoria(mapa* m,player *p);

void cheat_debug(mapa *m);

int checa_validade_e_atualiza_diamantes (mapa *m,player *p, int linha, int coluna,audio *sons);

int pode_empurrar_pedra(mapa *m, int linha, int coluna, int direcao);

void checa_pedras_na_cabeca(mapa *m, player *p);

void derruba_pedras(mapa *m,player *p,audio *sons);

void player_move(mapa *m,player *p,audio *sons,int direcao);

void imprime_mapa(mapa *m,player *p,sprites *s);

void imprime_perdeu(mapa *m,player *p,ALLEGRO_FONT *font,audio *sons);

int checa_debug(int debug[5]);

void destroi_mapa(mapa *m);

void destroi_tudo();

int abertura();

void pausa();

void jogando();

#endif

