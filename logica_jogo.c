#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "allegro5/allegro_ttf.h"
#include "boulder.h"
#include "sprites.h"
#include "audio.h"
#include "logica_jogo.h"


int checa_vitoria(mapa* m,player *p){
    //checa se o player esta na saida, se sim ele ganhou
    if (p->linha_player==m->linha_saida && p->coluna_player==m->coluna_saida)
        return 1;
    return 0;

}


void cheat_debug(mapa *m){
    //o cheatcode transforma todas as pedras e inimigos em diamantes

    int i,j;

    m->tempo=m->tempo+500;


    for(i=0;i<m->linhas;i++){
        for(j=0;j<m->colunas;j++){
            if(m->matriz[i][j]!= MURO && m->matriz[i][j]!=PLAYER && m->matriz[i][j]!=TUNEL && m->matriz[i][j]!=BORDA && m->matriz[i][j]!=TERRA && m->matriz[i][j]!=SAIDA){
                m->matriz[i][j]=DIAMANTE;
            }
        }
    }
}

int checa_validade_e_atualiza_diamantes (mapa *m,player *p, int linha, int coluna,audio *sons){
    //checa se pode ir para a nova posicao e se pode, atualiza os diamantes do player
    if ((m->matriz[linha][coluna] == TUNEL) ||(m->matriz[linha][coluna] == TERRA) ||
       (m->matriz[linha][coluna] == DIAMANTE) || (m->matriz[linha][coluna] == SAIDA && m->saida_aberta==1)){
        if(m->matriz[linha][coluna] == DIAMANTE){
            p->diam++;
            p->pontos=p->pontos+m->pontos_por_diam;
            if(p->diam == m->meta_diam){
                al_play_sample(sons->diamante,2,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                m->saida_aberta=1;
            }

            al_play_sample(sons->diamante,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);

        }
        return 1;
    }

    else
        return 0;

}

int pode_empurrar_pedra(mapa *m, int linha, int coluna, int direcao){
    //checa se pode empurrar a pedra atual
    if (coluna > 1 && coluna<m->colunas-1){
        if(direcao == ESQ  && m->matriz[linha][coluna]==PEDRA && m->matriz[linha][coluna-1]==TUNEL)
            return 1;

        if (direcao == DIR  && m->matriz[linha][coluna]==PEDRA && m->matriz[linha][coluna+1]==TUNEL)
            return 1;
    }

    
    return 0;
}

int monstro_pode_andar(mapa *m, player *p,int i, int j, int direcao){
    //checa se o monstro pode andar para a direcao desejada
    
    if((direcao == DIR) && (m->matriz[i][j+1]==TUNEL || m->matriz[i][j+1]==PLAYER))
        return 1;

    else if((direcao == ESQ) && (m->matriz[i][j-1]==TUNEL || m->matriz[i][j-1]==PLAYER))
        return 1;

    else if((direcao == SOBE) && (m->matriz[i-1][j]==TUNEL || m->matriz[i-1][j]==PLAYER))
        return 1;

    else if((direcao == DESCE) && (m->matriz[i+1][j]==TUNEL || m->matriz[i+1][j]==PLAYER))
        return 1;

    return 0;
}


void move_monstros(mapa *m,player *p,audio *sons){
    //move os monstros
    int i,j,direcao;

    srand(time(0));

    for(i=1;i<m->linhas-1;i++){
        for(j=1;j<m->colunas-1;j++){

            if(m->matriz[i][j]==VAGALUME || m->matriz[i][j]==BORBOLETA){
                direcao=(rand()%4+1);
                if (monstro_pode_andar(m,p,i,j,direcao)){
                    if(direcao == DIR)
                        m->matriz[i][j+1]=m->matriz[i][j];

                    if(direcao == ESQ)
                        m->matriz[i][j-1]=m->matriz[i][j];

                    if(direcao == SOBE)
                        m->matriz[i-1][j]=m->matriz[i][j];

                    if(direcao == DESCE)
                        m->matriz[i+1][j]=m->matriz[i][j];

                    m->matriz[i][j]=TUNEL;

                    
                }
            }
        }
    }
}

void checa_pedras_na_cabeca(mapa *m, player *p){
    //checa se o jogador tem uma pedra na cabeca (para evitar que ela caia indevidamente)
    if(m->matriz[p->linha_player-1][p->coluna_player] == PEDRA || m->matriz[p->linha_player-1][p->coluna_player] == DIAMANTE){
        p->embaixo_pedra=1;
    }
    else
        p->embaixo_pedra=0;
}


void derruba_pedras(mapa *m,player *p,audio *sons){
    //percorre todo o mapa derrubando as pedras
    int i,j;

        //percorreas linhas e colunas de tras para frente
        //(para evitar que a mesma pedra caia duas vezes)

        if(m->frames%2==0){

            for(i=m->linhas-1;i>0;i--){
                    for(j=m->colunas-1;j>0;j--){


                if((m->matriz[i][j]==PEDRA|| m->matriz[i][j]==DIAMANTE) && (m->matriz[i+1][j]==PEDRA|| m->matriz[i+1][j]==DIAMANTE) 
                && (m->matriz[i][j+1]==TUNEL && (m->matriz[i+1][j+1]== TUNEL||m->matriz[i+1][j+1]== PLAYER))){
                    //cai direita
                    if(!(p->embaixo_pedra)){
                        m->matriz[i+1][j+1]=m->matriz[i][j];
                        m->matriz[i][j]=TUNEL;
                        al_play_sample(sons->queda, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    }



                }


                if((m->matriz[i][j]==PEDRA|| m->matriz[i][j]==DIAMANTE) && (m->matriz[i+1][j]==PEDRA|| m->matriz[i+1][j]==DIAMANTE) 
                && (m->matriz[i][j-1]==TUNEL && (m->matriz[i+1][j-1]== TUNEL||m->matriz[i+1][j-1]== PLAYER))){
                    //cai esquerda
                    if(!(p->embaixo_pedra)){
                        m->matriz[i+1][j-1]=m->matriz[i][j];
                        m->matriz[i][j]=TUNEL;
                        al_play_sample(sons->queda, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    }

                }


                if ((m->matriz[i][j]==PEDRA || m->matriz[i][j]==DIAMANTE) && (m->matriz[i+1][j]==TUNEL || m->matriz[i+1][j]==PLAYER)){
                    //cai para baixo
                    if(!(p->embaixo_pedra)){
                        m->matriz[i+1][j]=m->matriz[i][j];
                        m->matriz[i][j]=TUNEL;
                        al_play_sample(sons->queda, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    }

                }
            }
        }
    }
}



void empurra_pedra(mapa *m,player *p, audio *sons,int linha, int coluna, int direcao){
    //se existe uma pedra sendo empurrada, faz essa acao
    if (pode_empurrar_pedra(m,linha,coluna,direcao)){

        if(direcao==DIR){
            m->matriz[linha][coluna+1]=PEDRA;

        }

        else{
            m->matriz[linha][coluna-1]=PEDRA;
        }

    }
}

void player_move(mapa *m,player *p,audio *sons,int direcao){
    //move o player e atualiza sua struct
    int linha_atual,coluna_atual;

    if(p->vivo){

        p->parado=0;
        p->sprite_movimento++;

        linha_atual=p->linha_player;
        coluna_atual=p->coluna_player;

        m->matriz[linha_atual][coluna_atual]=TUNEL;


        if (direcao == SOBE && checa_validade_e_atualiza_diamantes(m,p,linha_atual-1,coluna_atual,sons)){
            p->linha_player--;
            p->direcao=SOBE;
            m->matriz[linha_atual-1][coluna_atual]=PLAYER;
            checa_pedras_na_cabeca(m,p);
            al_play_sample(sons->anda, 0.75, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);



        }

        else if (direcao == DESCE && checa_validade_e_atualiza_diamantes(m,p,linha_atual+1,coluna_atual,sons)){
            p->linha_player++;
            p->direcao=SOBE;
            m->matriz[linha_atual+1][coluna_atual]=PLAYER;
            checa_pedras_na_cabeca(m,p);
            al_play_sample(sons->anda, 0.75, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);


        }

        else if (direcao == ESQ && (checa_validade_e_atualiza_diamantes(m,p,linha_atual,coluna_atual-1,sons) || pode_empurrar_pedra(m,linha_atual,coluna_atual-1,ESQ)) ){
            empurra_pedra(m,p,sons,linha_atual,coluna_atual-1,ESQ);
            p->coluna_player--;
            p->direcao=ESQ;
            m->matriz[linha_atual][coluna_atual-1]=PLAYER;
            checa_pedras_na_cabeca(m,p);
            al_play_sample(sons->anda, 0.75, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);


        }

        else if (direcao == DIR && (checa_validade_e_atualiza_diamantes(m,p,linha_atual,coluna_atual+1,sons) || pode_empurrar_pedra(m,linha_atual,coluna_atual+1,DIR))){
            empurra_pedra(m,p,sons,linha_atual,coluna_atual+1,DIR);
            p->coluna_player++;
            p->direcao=DIR;
            m->matriz[linha_atual][coluna_atual+1]=PLAYER;
            checa_pedras_na_cabeca(m,p);
            al_play_sample(sons->anda, 0.75, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);


        }

        else
            m->matriz[linha_atual][coluna_atual]=PLAYER;
    }
}

int checa_debug(int debug[5]){
    //checa se o cheatcode foi ativado
    int i, valido;

    valido=1;
    for(i=0;i<5;i++){
        if(debug[i]!= 1)
            valido=0;
    }

    return valido;
}


void checa_morte(mapa *m, player *p, audio *s){
    //checa se o player sumiu do mapa (foi esmagado)
    if(m->matriz[p->linha_player][p->coluna_player]!= PLAYER)
        p->vivo=0;
}

