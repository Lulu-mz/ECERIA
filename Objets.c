//
// Created by viola on 20/03/2025.
//

#include "Objets.h"



Arbre* creerArbre(int x, int y) {
    Arbre* arbre = malloc(1 * sizeof(Arbre));
    arbre->image = al_load_bitmap("../Assets/Cats/Objects/biome.png");
    arbre->pointsVie = 100;
    arbre->x = x;
    arbre->y = y;
    return arbre;
}

void destroyArbre(Arbre* arbre) {
    free(arbre);
}

void taperArbre(Carte* carte, Joueur* joueur, int x, int y, int x2, int y2) {
    Arbre* arbres[2];
    arbres[0] = carte->map[y][x].arbre;
    arbres[1] = carte->map[y2][x2].arbre;
    for(int i = 0; i<2; i++) {
        if (arbres[i]!=NULL) {
            //on regarde si y'a un arbre
            arbres[i]->pointsVie -= joueur->degats;
            if (arbres[i]->pointsVie <= 0) {
                carte->map[arbres[i]->x][arbres[i]->y].marchable = true;
                carte->map[arbres[i]->x][arbres[i]->y].arbre = NULL;
                destroyArbre(arbres[i]);
                arbres[i] = NULL;

            }
        }
    }
}

