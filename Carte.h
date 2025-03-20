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
typedef struct arbre {
    int pointsVie;
    ALLEGRO_BITMAP* image;
}Arbre;

typedef enum type {
    HERBE,
    HERBE2,
    FLEUR
} Type;

typedef struct c {
    Type typeCase;
    ALLEGRO_BITMAP* image;
    bool marchable;
    Arbre* arbre;
}Case;

typedef struct carte {
    Case** map;
    int largeur;
    int hauteur;
}Carte;

Carte* chargerCarte(int mapWidth, int mapHeight);
void afficherCarte(Carte* carte);
void destroyCarte(Carte* carte);

void ajouterArbres(Carte* carte);
#endif //CARTE_H
