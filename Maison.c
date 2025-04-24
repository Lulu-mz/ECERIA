//
// Created by viola on 23/04/2025.
//

#include "Maison.h"

#include <stdio.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/bitmap_io.h>

#include "Constantes.h"

Maison* creerMaison(TypeMaison type) {
    Maison* m = malloc(1*sizeof(Maison));
    m->type = type;
    m->image = al_load_bitmap("../Assets/Houses/house.png");
    if (!m->image) {
        printf("Erreur de chargement de l'image\n");
        exit(1);
    }
    switch (type) {
        case MAISON_0 :
            m->largeur = 3;
            m->hauteur = 3;
            m->valeur = 3;
        break;
        case MAISON_1 :
            m->largeur = 5;
            m->hauteur = 3;
            m->valeur = 4;
        break;
    }
    return m;
}


void afficherMaison(Maison* m, int pos_x, int pos_y) {
    int sw = 16;
    int sh = 16;
    int sx = 0;
    int sy = 0;
    char buffer[50];
    sprintf(buffer, "../Save/pattern/maison_%d.txt",m->type);
    FILE* fichier = fopen(buffer, "r");
    for(int i = 0; i < m->hauteur; i++) {
        for(int j = 0; j < m->largeur; j++) {
            int valeur = -1;
            fscanf(fichier, "%d", &valeur);
            sx = valeur%4 * 16;
            sy = (valeur/4 + 4) * 16;
            float dx = pos_x + j;
            float dy = pos_y + i;
            al_draw_scaled_bitmap(m->image, sx,sy,sw,sh,dx*TILE_SIZE,dy*TILE_SIZE, TILE_SIZE, TILE_SIZE, 0);
        }
    }
    fclose(fichier);
}

void destroyMaison(Maison* m) {
    free(m);
    m = NULL;
}