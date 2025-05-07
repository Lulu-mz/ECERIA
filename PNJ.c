//
// Created by viola on 07/05/2025.
//
#include "Constantes.h"
#include "PNJ.h"

//TODO : s'arranger avec pnj->audio pour les audios...

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
            pnj->audio = NULL;
        break;
        case POULE :
            pnj->image = al_load_bitmap("../Assets/Cats/Characters/Poule.png");
            pnj->sx = 0;
            pnj->sy = 0;
            pnj->sw = 16;
            pnj->sh = 16;
            pnj->audio = NULL;
        break;
        default :
            // pnj->image = al_load_bitmap("../Assets/Spritesheet/character.png");
            // pnj->sx = 0;
            // pnj->sy = 5;
            // pnj->sw = 16;
            // pnj->sh = 16;
            // pnj->audio = NULL;
        break;
    }
    return pnj;
}

void destroyPnj(Pnj* pnj) {
    free(pnj);
    pnj = NULL;
}

void afficherPnj(Pnj* pnj, int x, int y) {
    //FIXME : Dommage que l'affichage ne peut pas se généraliser...
    if(pnj->type == VACHE) {
        al_draw_scaled_bitmap(pnj->image, 1+pnj->sx*(16+1), 1+pnj->sy*(16+1), pnj->sw, pnj->sh, (x*TILE_SIZE+4)-TILE_SIZE, y*TILE_SIZE-TILE_SIZE, pnj->sw*2, pnj->sh*2,0);
    }
    else if(pnj->type == POULE) {
        al_draw_scaled_bitmap(pnj->image, 1+pnj->sx*(16+1), 1+pnj->sy*(16+1), pnj->sw, pnj->sh, (x*TILE_SIZE+4), y*TILE_SIZE, pnj->sw*2, pnj->sh*2,0);
    }
}