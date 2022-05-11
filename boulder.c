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

audio *sons;

ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_TIMER* timer;
ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;
ALLEGRO_FONT* font;
ALLEGRO_EVENT event;

bool redraw;
bool done;

int estado;


unsigned char key[ALLEGRO_KEY_MAX];

int debug[5]={0,0,0,0,0};


void must_init(bool test, const char *description)
//checa se o objeto inicializou corretamente
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}


void disp_init()
//inicializa o display
{
    disp = al_create_display(DISP_L, DISP_A);
    must_init(disp, "display");

    buffer = al_create_bitmap(LARGURA, ALTURA);
    must_init(buffer, "bitmap buffer");
}


void disp_pre_draw()
{
    al_set_target_bitmap(buffer);
}

void disp_post_draw()
//imprime o buffer
{
    al_set_target_backbuffer(disp);
    al_draw_scaled_bitmap(buffer, 0, 0, LARGURA, ALTURA, 0, 0, DISP_L, DISP_A, 0);

    al_flip_display();
}


void inicia_tudo(){
    //inicializacoes geral do allegro
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    timer = al_create_timer(1.0 / 10.0);
    must_init(timer, "timer");

    queue = al_create_event_queue();
    must_init(queue, "queue");

    must_init(al_install_audio(), "audio");
    al_reserve_samples(1);

    al_init_acodec_addon();
    must_init(al_reserve_samples(16), "reserve samples");    //ate 16 sons simultaneos

    must_init(al_init_image_addon(), "image addon");
    must_init(al_init_font_addon(),"font addon");
    must_init(al_init_ttf_addon(), "ttf addon");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    disp = al_create_display(DISP_L, DISP_A);
    must_init(disp, "display");

    buffer = al_create_bitmap(LARGURA,ALTURA);
    must_init(buffer,"buffer");

    font = al_create_builtin_font();
    font = al_load_ttf_font("resources/8bits.ttf", 12, 0);
    must_init(font, "font");


    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    disp_init();

    done = false;
    redraw = true;
    
    #define KEY_SEEN     1
    #define KEY_RELEASED 2

    memset(key, 0, sizeof(key));

    al_start_timer(timer);

}

void le_mapa_e_cria_structs(mapa *m, player *p,int atual){
    //le o arquivo de mapas e inicializa as structs

    FILE *arq;
    int i,j;

    switch(atual){
        case 1: 
            arq = fopen("resources/mapas/mapa1.txt", "rb");
            break;

        case 2:
            arq = fopen("resources/mapas/mapa2.txt", "rb");
            break;

        case 3:
            arq = fopen("resources/mapas/mapa3.txt", "rb");
            break;

        case 4:
            arq = fopen("resources/mapas/mapa4.txt", "rb");
            break;

        case 5:
            arq = fopen("resources/mapas/mapa5.txt", "rb");
            break;
        
        case 6:
            arq = fopen("resources/mapas/mapa6.txt", "rb");
            break;
        
        case 7:
            arq = fopen("resources/mapas/mapa7.txt", "rb");
            break;

        case 8:
            arq = fopen("resources/mapas/mapa8.txt", "rb");
            break;

        case 9:
            arq = fopen("resources/mapas/mapa9.txt", "rb");
            break;

        case 10:
            arq = fopen("resources/mapas/mapa10.txt", "rb");
            break;

        case 11:
            arq = fopen("resources/mapas/mapa11.txt", "rb");
            break;

        case 12:
            arq = fopen("resources/mapas/mapa12.txt", "rb");
            break;

        default:
            arq = fopen("resources/mapas/mapa1.txt", "rb");
    }



    if (!arq)
        perror("Erro! O arquivo nao existe!");
        //caso o arquivo nao exista, devolve erro
    

    else{
        fscanf(arq,"%i %i\n",&m->linhas,&m->colunas);
        fscanf(arq,"%i %i %i\n",&m->meta_diam,&m->pontos_por_diam,&m->tempo);

        p->pontos=0;
        p->diam=0;
        p->vivo=1;
        p->sprite_parado=0;
        p->sprite_movimento=0;
        p->parado=1;
        p->direcao=DESCE;
        p->embaixo_pedra=1;

        m->saida_aberta=0;
        m->frames=0;
        m->matriz = malloc (m->colunas * sizeof (int*));

        for (i=0; i < m->linhas; i++)
            m->matriz[i] = malloc (m->colunas * sizeof (int)) ;

        for(i=0;i<m->linhas;i++){
            for(j=0;j<m->colunas;j++){
                fscanf(arq,"%i ",&m->matriz[i][j]);

                if(m->matriz[i][j] == PLAYER){
                    //se o player esta na posicao atual, atualiza suas infos
                    p->coluna_player=j;
                    m->coluna_player=j;
                    p->linha_player=i;
                    m->linha_player=i;
                }

                if(m->matriz[i][j]==SAIDA){
                    //se o local atual eh a saida, indica na struct mapa
                    m->linha_saida=i;
                    m->coluna_saida=j;
                }
            }
        }
    fclose(arq);
    }
}



void imprime_ganhou(mapa* m,player *p, audio *sons,ALLEGRO_FONT *font){
    //imprime na tela que o jogador ganhou
    if (p->linha_player==m->linha_saida && p->coluna_player==m->coluna_saida){
        if(m->tempo != 0){
            p->pontos=p->pontos+m->tempo;
            m->tempo=0;
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_textf(font, al_map_rgb(255, 255, 255), m->colunas,m->linhas,0, "level clear  score:%i",p->pontos);
        al_play_sample(sons->ganhou,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);

    }
}


void imprime_mapa(mapa *m,player *p,sprites *s){
    //imprime o mapa
    int lin,col,i,j,l_atual,c_atual;

    lin=m->linhas;
    col=m->colunas;

    if(m->frames%3==0){
        p->sprite_parado++;
    }

    if(p->sprite_parado>6)
        p->sprite_parado=0;

    if(p->sprite_movimento>6)
        p->sprite_movimento=0;


    for(i=0;i<lin;i++){
        for(j=0;j<col;j++){
            l_atual=(i+1)*16;
            c_atual=j*16;

            switch (m->matriz[i][j]){
                case TUNEL:
  	  	            al_draw_scaled_bitmap(s->tunel, 0, 0, 16, 16, c_atual, l_atual, 16, 16, 0);
                    break;

                case TERRA:
  	  	            al_draw_scaled_bitmap(s->terra, 0, 0, 16, 16, c_atual, l_atual, 16, 16, 0);
                    break;

                case MURO:
  	  	            al_draw_scaled_bitmap(s->muro, 0, 0, 16, 16, c_atual, l_atual, 16, 16, 0);
                    break;
                
                case PEDRA:
  	  	            al_draw_scaled_bitmap(s->pedra, 0, 0, 16, 16, c_atual, l_atual, 16, 16, 0);
                    break;

                case DIAMANTE:
  	  	            al_draw_scaled_bitmap(s->diamante[m->frames%8], 0, 0, 16, 16, c_atual, l_atual, 16, 16, 0);
                    break;

                case PLAYER:
                    if(p->direcao==SOBE){
  	  	                al_draw_scaled_bitmap(s->personagem_e[p->sprite_movimento], 0, 0, 16, 16, c_atual, l_atual, 16, 16, 0);
                    }

                    if(p->direcao==ESQ){
  	  	                al_draw_scaled_bitmap(s->personagem_e[p->sprite_movimento], 0, 0, 16, 16, c_atual, l_atual, 16, 16, 0);

                    }

                    if(p->direcao==DIR)
                        al_draw_scaled_bitmap(s->personagem_d[p->sprite_movimento], 0, 0, 16, 16, c_atual, l_atual, 16, 16, 0);

                    if(p->direcao==DESCE)
                        al_draw_scaled_bitmap(s->personagem[p->sprite_parado], 0, 0, 16, 16, c_atual, l_atual, 16, 16, 0);


                    break;

                case BORDA:
  	  	            al_draw_scaled_bitmap(s->borda, 0, 0, 16, 16, c_atual, l_atual, 16, 16, 0);
                    break;

                case VAGALUME:
                    al_draw_scaled_bitmap(s->vagalume[m->frames%4],0,0,16,16,c_atual,l_atual,16,16,0);
                    break;

                case BORBOLETA:
                    al_draw_scaled_bitmap(s->borboleta[m->frames%4],0,0,16,16,c_atual,l_atual,16,16,0);
                    break;

                case AMOEBA:
                    al_draw_scaled_bitmap(s->amoeba[m->frames%4],0,0,16,16,c_atual,l_atual,16,16,0);
                    break;

                case SAIDA:
                    if(m->saida_aberta==0)
  	  	                al_draw_scaled_bitmap(s->borda, 0, 0, 16, 16, c_atual, l_atual, 16, 16, 0);
                    else
                        al_draw_scaled_bitmap(s->saida,0,0,16,16,c_atual,l_atual,16,16,0);
                    break;

                case MURO_MAGICO:
                    al_draw_scaled_bitmap(s->muro_magico[m->frames%4],0,0,16,16,c_atual,l_atual,16,16,0);
                    break;

                }
        }
    }

}


void imprime_perdeu(mapa *m,player *p,ALLEGRO_FONT *font,audio *sons){
    //imprime a tela mostrando que o usuario perdeu
    if(!checa_vitoria(m,p)){
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_textf(font, al_map_rgb(255, 255, 255), m->linhas, m->colunas, 0, "PERDEU score:%i",p->pontos);
        al_play_sample(sons->morreu,2,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
    }

}



void destroi_mapa(mapa *m){
    //libera a memoria do mapa
    int i;

    for (i=0; i < m->linhas; i++)
        free (m->matriz[i]) ;

    free (m->matriz) ;
    free(m);
}

void destroi_player(player *p){
    //libera player
    free(p);
}


void destroi_tudo(mapa *m, audio *sons, sprites *s, player *p){
    //destroi tudo que foi usado
    destroi_sprites(s);
    destroi_sons(sons);
    destroi_mapa(m);
    destroi_player(p);

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    exit(1);
}

int abertura(){
    //tela de abertura
    ALLEGRO_BITMAP* abertura = al_load_bitmap("resources/abertura.png");
    must_init(abertura, "abertura");

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                return JOGANDO;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                al_destroy_bitmap(abertura);
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 500, 0, 0, "Aperte qualquer botao para comecar!");

            al_draw_bitmap(abertura, 350, 100, 0);

            al_flip_display();

            redraw = false;
            return ABERTURA;
        }
    }


    al_destroy_bitmap(abertura);
    return JOGANDO;
}




void pausa(){
    //tela de pausa

    bool done = false;
    bool redraw = true;


    al_flush_event_queue(queue);
    al_start_timer(timer);
  
    while(1){

  	    al_wait_for_event(queue, &event);
  	  
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                redraw = false;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;

        }

        if(key[ALLEGRO_KEY_H] || key[ALLEGRO_KEY_F1]){
            estado = JOGANDO;
            done= true;
        

        }

        else if(key[ALLEGRO_KEY_ESCAPE]){
            estado = FECHOU;
            done = true;
        }

        if(done)
            break;

        
        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 400, 0, 0, "Ande com as setas, troque de nivel com PageUp e PageDown");
            al_draw_text(font, al_map_rgb(255, 255, 255), 400, 50, 0, "Pegue os diamantes ate a porta se abrir, evite os monstros e saia pela saida");
            al_draw_text(font, al_map_rgb(255, 255, 255), 400, 100, 0, "Cheatcode: debug");
            al_draw_text(font, al_map_rgb(255, 255, 255), 400, 150, 0, "Aperte h ou f1 para voltar a jogar");

            al_flip_display();

            redraw = true;
        }

    }
}


void jogando(){
    //roda a logica do jogo
    mapa *m;
    player *p;
    sprites *s;

    int atual,novo;

    m=malloc(sizeof(mapa));
    p=malloc(sizeof(player));

    atual=1;
    novo=atual;

    le_mapa_e_cria_structs(m,p,atual);

    s=separa_sprites();
    sons=separa_sons();

    al_set_audio_stream_playmode(sons->musica, ALLEGRO_PLAYMODE_LOOP);


    al_attach_audio_stream_to_mixer(sons->musica, al_get_default_mixer());

    while(1)
    {   
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                p->parado=1;
                p->direcao=DESCE;

                if(key[ALLEGRO_KEY_UP]){
                    if(m->linha_player>0){
                        player_move(m,p,sons,SOBE);
                    }
                }

                

                if(key[ALLEGRO_KEY_DOWN]){
                    if(m->linha_player<m->linhas){
                        player_move(m,p,sons,DESCE);
                    }
                }


        
                if(key[ALLEGRO_KEY_LEFT]){
                    if(m->coluna_player>0){
                        player_move(m,p,sons,ESQ);
                    }
                }

                if(key[ALLEGRO_KEY_RIGHT])
                    if(m->coluna_player<m->colunas){
                        player_move(m,p,sons,DIR);
                    }


                if(key[ALLEGRO_KEY_ESCAPE]){
                    destroi_tudo(m,sons,s,p);
                    done = true;
                }


                if(key[ALLEGRO_KEY_D])
                    debug[0]=1;

                if(key[ALLEGRO_KEY_E])
                    debug[1]=1; 

                if(key[ALLEGRO_KEY_B])
                    debug[2]=1;

                if(key[ALLEGRO_KEY_U])
                    debug[3]=1;    

                if(key[ALLEGRO_KEY_G]){
                    debug[4]=1;
                    if(checa_debug(debug))
                        cheat_debug(m);
                }

                if(key[ALLEGRO_KEY_PGUP]){
                    if(novo>1){
                        novo--;
                    }
                }

                if(key[ALLEGRO_KEY_PGDN]){
                    if(novo<12){
                        novo++;
                    }
                }

                if(key[ALLEGRO_KEY_H] || key[ALLEGRO_KEY_F1]){
                    pausa();
                }
                

                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;

                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            m->frames++;

            if(m->frames%12==0 && m->tempo> 0)
                m->tempo--;

            if(m->tempo <= 0)
                p->vivo=0;

            
            disp_pre_draw();
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "%i %i %i %i %i", m->meta_diam,m->pontos_por_diam,p->diam,p->pontos,m->tempo);


            if(p->vivo){
                if(checa_vitoria(m,p)){
                    p->pontos=p->pontos+m->tempo;
                    m->tempo=0;
                }

                else
                    {
                    if(atual!= novo){
                        atual=novo;
                        le_mapa_e_cria_structs(m,p,atual);
            }
                        move_monstros(m,p,sons);
                        derruba_pedras(m,p,sons);
                        checa_morte(m,p,sons);
                        imprime_mapa(m,p,s);
                    }
            }

            else{
                if(checa_vitoria(m,p)){
                    imprime_ganhou(m,p,sons,font);
                    novo++;
                }

                else{
                    imprime_perdeu(m,p,font,sons);
                }

            if(atual!= novo){
                atual=novo;
            }
            le_mapa_e_cria_structs(m,p,atual);
            

            }

            

            disp_post_draw();


            al_flip_display();

            redraw = false;
        }
    }

    destroi_tudo(m,sons,s,p);
}

