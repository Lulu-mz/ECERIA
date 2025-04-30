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
    FLEUR,
    SOL,
    NOIR,
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
    int sx;
    int sy;
}Case;

typedef struct carte {
    Case** map;
    int largeur;
    int hauteur;
    struct carte** carte_maison;
    int nbMaison;
}Carte;

Carte* chargerCarte(int mapWidth, int mapHeight, int pos_i, int pos_j);
void afficherCarte(Carte* carte);
void destroyCarte(Carte* carte);
void chargerBiome(Carte* carte, int pos_i, int pos_j);
Carte* creerCarte(int w, int h);
int saveCarte(Carte* carte, int pos_i, int pos_j);
int genererMaisons(Carte* carte);
int compterMaison(Carte* carte, int pos_i, int pos_j);
void genererGrassLand(Carte* carte);
int isValidSpace(Carte* carte, int x, int y);
void attribuateIDHouse(Carte* carte);
void saveInterieurMaison(Carte* carte, int pos_i, int pos_j, int id);
Carte* chargerInterieurMaison(int h, int w, int pos_i, int pos_j, int id);
Carte* genererInterieurMaison(int h, int w);
#endif //CARTE_H
