//
// Created by viola on 20/03/2025.
//

#ifndef OBJETS_H
#define OBJETS_H
#include <allegro5/bitmap.h>

typedef enum grassLandType {
    ARBRE,
    ROCHER,
    ARBRE_F_G,
    ARBRE_F_D
}GrassLandType;

typedef struct grassLand {
    int pointsVie;
    ALLEGRO_BITMAP* image;
    int x, y;
    GrassLandType type;
    int sx, sy;
    int h, w;
    int valeur;
}GrassLand;

GrassLand* creerGrassLand(int x, int y, GrassLandType type);
void afficherGrassLand(GrassLand *gl, int pos_x, int pos_y);
void destroyGrassLand(GrassLand* gl);
#endif //OBJETS_H
