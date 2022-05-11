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



int main(){
    inicia_tudo();

    int estado = ABERTURA;

    while(estado==ABERTURA)
        estado=abertura();

    //fica na tela de inicio ate uma tecla ser apertada

    while(1){
        jogando();
    }

    return 0;
}