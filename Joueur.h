//
// Created by viola on 04/02/2025.
//
#ifndef JOUEUR_H
#define JOUEUR_H
#include "Constantes.h"
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/display.h>
#include "Carte.h"

typedef enum direction {
    DROITE, GAUCHE, HAUT, BAS, STATIQUE
}Direction;

typedef struct j {
    float speed;
    float x1, y1;
    float x2, y2;
    Direction direction;
    ALLEGRO_BITMAP* image;
    float sprite_x;
    float sprite_y;
    int degats;
}Joueur;

Joueur* createJoueur();
void destroyJoueur(Joueur* joueur);
void afficherJoueur(Joueur* joueur, int curentFrame);
void deplacerJoueur(Joueur* joueur, Carte* carte);
void action(Joueur* joueur, Carte* carte);
#endif //JOUEUR_H
