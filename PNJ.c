//
// Created by viola on 07/05/2025.
//
#include "Constantes.h"
#include "PNJ.h"


Pnj* creerPnj(TypePnj type) {
    Pnj* pnj = malloc(1*sizeof(Pnj));
    pnj->type = type;
    pnj->val = 20+(int)pnj->type;
    switch (type) {
        case VACHE :
            pnj->image = al_load_bitmap("../Assets/Cats/Characters/Vache.png");
            pnj->sx = 0;
            pnj->sy = 0;
            pnj->sw = 32;
            pnj->sh = 32;
            pnj->audio = al_load_sample("../Sounds/NPC/cow.wav");
        break;
        case POULE :
            pnj->image = al_load_bitmap("../Assets/Cats/Characters/Poule.png");
            pnj->sx = 0;
            pnj->sy = 0;
            pnj->sw = 16;
            pnj->sh = 16;
            pnj->audio = al_load_sample("../Sounds/NPC/chicken.wav");
        break;
        case PERSO_0 :
            pnj->image = al_load_bitmap("../Assets/Cats/Characters/Humans/NPC.png");
            pnj->sx = 8;
            pnj->sy = 4;
            pnj->sw = 16;
            pnj->sh = 24;
            pnj->audio = al_load_sample("../Sounds/NPC/hey.wav");
            break;
        default :
        break;
    }
    return pnj;
}

void destroyPnj(Pnj* pnj) {
    free(pnj);
    pnj = NULL;
}

void afficherPnj(Pnj* pnj, int x, int y, int currentFrame) {
    int max = 2;
    if(pnj->type == VACHE) {
        max = currentFrame%3;
        al_draw_scaled_bitmap(pnj->image, (pnj->sx + max)*32, pnj->sy*16, pnj->sw, pnj->sh, (x*TILE_SIZE+4)-TILE_SIZE, y*TILE_SIZE-TILE_SIZE, pnj->sw*2, pnj->sh*2,0);
    }
    else if(pnj->type == POULE) {
        max = currentFrame%2;
        al_draw_scaled_bitmap(pnj->image, (pnj->sx + max)*16, pnj->sy*16, pnj->sw, pnj->sh, (x*TILE_SIZE+4), y*TILE_SIZE, pnj->sw*2, pnj->sh*2,0);
    }
    else if(pnj->type == PERSO_0) {
        max = currentFrame%5;
        al_draw_scaled_bitmap(pnj->image, pnj->sx + max*32, pnj->sy, pnj->sw, pnj->sh, x*TILE_SIZE, y*TILE_SIZE - 8, pnj->sw*2, pnj->sh*2,0);
    }
}