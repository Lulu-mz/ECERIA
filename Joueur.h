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
#include "Inventaire.h"

typedef enum direction {
    DROITE, GAUCHE, HAUT, BAS, STATIQUE
}Deplacement;

typedef enum regard {
    RIGHT, LEFT, UP, DOWN
}Watching;

typedef struct j {
    float speed;
    float l,h;
    float x1, y1;
    float x2, y2;
    Deplacement direction;
    ALLEGRO_BITMAP* image;
    float sprite_x;
    float sprite_y;
    int degats;
    Watching regard;
    Inventaire* inventaire;
    int pos_i; //positions section
    int pos_j; //positions section
    int inHouse;
}Joueur;

Joueur* createJoueur();
Joueur* chargerJoueur();
void saveJoueur(Joueur* joueur);
void destroyJoueur(Joueur* joueur);
void afficherJoueur(Joueur* joueur, int curentFrame);
void action(Joueur* joueur, Carte* carte, int* px, int* py);
void destroyInventaire(Inventaire* inv);
Item* taperGrassLand(Carte* carte, Joueur* joueur, int x, int y);
void actionGrassLand(Carte* carte,Joueur* joueur, int x, int y);
void ouvrirPorte(Carte* carte, Joueur* joueur, int id);
void deplacerJoueurDevantMaison(Carte* carte, Joueur* joueur, int id_m);
void actionPorte(Carte* carte, Joueur* joueur, int x, int y);
void actionPnj(Carte* carte, int x, int y);
#endif //JOUEUR_H
