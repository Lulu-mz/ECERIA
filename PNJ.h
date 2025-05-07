//
// Created by viola on 07/05/2025.
//

#ifndef PNJ_H
#define PNJ_H
#include <allegro5/allegro_audio.h>
#include <allegro5/bitmap.h>

typedef enum type_pnj {
    VACHE,
    POULE,
}TypePnj;

typedef struct pnj {
    ALLEGRO_BITMAP* image;
    int sx;
    int sy;
    int sw;
    int sh;
    ALLEGRO_SAMPLE* audio;
    TypePnj type;
    int val;
}Pnj;

//TODO : faire les audio qd on fait une action
//TODO : faire les actions qd on parle à un pnj

Pnj* creerPnj(TypePnj type);
Pnj* chargerPnj();
void destroyPnj(Pnj* pnj);
void afficherPnj(Pnj* pnj, int x, int y);
#endif //PNJ_H
