//
// Created by viola on 04/03/2025.
//

#ifndef CARTE_H
#define CARTE_H
#include <allegro5/bitmap.h>
#include <stdio.h>
#include <time.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/allegro_image.h>

#include "Biome.h"
#include "Maison.h"


//TODO : faire une structure image


typedef enum type {
    HERBE,
    HERBE2,
    FLEUR
} Type;

typedef struct c {
    Type typeCase;
    ALLEGRO_BITMAP* image;
    bool marchable;
    bool vide;
    GrassLand* grassLand;
    int size;
    Maison* maison;
    Porte* porte;
}Case;

typedef struct carte {
    Case** map;
    int largeur;
    int hauteur;
}Carte;

Carte* chargerCarte(int mapWidth, int mapHeight, int pos_i, int pos_j);
void afficherCarte(Carte* carte);
void destroyCarte(Carte* carte);
void chargerGrassLand(Carte* carte, int pos_i, int pos_j);
Carte* creerCarte(int w, int h);
int saveCarte(Carte* carte, int pos_i, int pos_j);
void genererMaisons(Carte* carte);
void genererGrassLand(Carte* carte);
#endif //CARTE_H
