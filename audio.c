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
#include "boulder.h"

audio* separa_sons(){
    audio* sons;
    sons=malloc(sizeof(audio));

    ALLEGRO_SAMPLE* anda = al_load_sample("resources/sons/anda.ogg");
    ALLEGRO_SAMPLE* diamante = al_load_sample("resources/sons/diamante.wav");
    ALLEGRO_SAMPLE* empurra = al_load_sample("resources/sons/empurra.wav");
    ALLEGRO_SAMPLE* morreu = al_load_sample("resources/sons/morreu.wav");
    ALLEGRO_SAMPLE* queda = al_load_sample("resources/sons/queda.ogg");
    ALLEGRO_SAMPLE* ganhou = al_load_sample("resources/sons/ganhou.wav");


    ALLEGRO_AUDIO_STREAM* musica = al_load_audio_stream("resources/sons/musica.ogg", 3, 1024);

    must_init(anda, "anda");
    must_init(diamante, "diamante");
    must_init(empurra, "empurra");
    must_init(morreu, "morreu");
    must_init(musica, "musica");
    must_init(queda,"queda");
    must_init(ganhou,"ganhou");


    sons->anda=anda;
    sons->diamante=diamante;
    sons->empurra=empurra;
    sons->morreu=morreu;
    sons->musica=musica;
    sons->queda=queda;
    sons->ganhou=ganhou;

    return sons;
}

void destroi_sons(audio* sons){
  al_destroy_sample(sons->anda);
  al_destroy_sample(sons->diamante);
  al_destroy_sample(sons->empurra);
  al_destroy_sample(sons->morreu);
  al_destroy_audio_stream(sons->musica);
  al_destroy_sample(sons->queda);
  al_destroy_sample(sons->ganhou);
}