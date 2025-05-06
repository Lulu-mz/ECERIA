//
// Created by viola on 19/02/2025.
//

#ifndef JEU_H
#define JEU_H
#include <allegro5/allegro.h>
#include <stdlib.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <time.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/events.h>
#include "Constantes.h"
#include <allegro5/keyboard.h>
#include <allegro5/mouse.h>
#include <stdio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Joueur.h"

typedef struct jeu {
    Carte*** sections;
    int mapSize;
    Joueur* joueur;
}Jeu;

//Bouton :
typedef struct bs {
    float x, y;         // Position initiale du bouton
    float width, height;
    bool hovered;
    bool clicked;
    float scale;
    float alpha;
} ButtonState;


//Menu :
Jeu* menu(ALLEGRO_EVENT_QUEUE *queue);
void afficherMenu(ALLEGRO_BITMAP* background, ALLEGRO_BITMAP* buttonImg, ButtonState *btnNewGame, ButtonState *btnLoadGame);
void afficherButton(ALLEGRO_BITMAP *buttonImg, ButtonState *btn);

//Jeu :
int animation();
void saveJeu(Jeu* jeu);
void destroyJeu(Jeu* jeu);
Jeu* nouvellePartie();
Jeu* chargerPartie();
Carte* getCurrentCarte(Jeu* jeu);
//Son dans le jeu :
int playSound(Jeu* jeu);
void cleanUp(ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_DISPLAY *window, Jeu *jeu, ALLEGRO_SAMPLE *sample);
//Joueur dans le jeu :
void deplacerJoueur(Jeu* jeu);
#endif //JEU_H
