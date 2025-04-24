//
// Created by viola on 23/04/2025.
//

#ifndef MAISON_H
#define MAISON_H
#include <allegro5/bitmap.h>

typedef enum type_m {
    MAISON_0,
    MAISON_1
}TypeMaison;

typedef  struct m {
    int largeur;
    int hauteur;
    ALLEGRO_BITMAP* image;
    TypeMaison type;
    int valeur;
}Maison;

Maison* creerMaison(TypeMaison type);
void afficherMaison(Maison* m, int pos_x, int pos_y);
void destroyMaison(Maison* m);
#endif //MAISON_H
