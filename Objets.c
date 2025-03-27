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

Item* taperArbre(Carte* carte, Joueur* joueur, int x, int y, int x2, int y2) {
    int taille = 1;
    int nb_bois = 0;
    Arbre* arbres[2];
    arbres[0] = carte->map[y][x].arbre;
    if (x!=x2 || y !=y2) {
        arbres[1] = carte->map[y2][x2].arbre;
        taille = 2;
    }
    for(int i = 0; i<taille; i++) {
        if (arbres[i]!=NULL) {
            //on regarde si y'a un arbre
            arbres[i]->pointsVie -= joueur->degats;
            nb_bois+=5;
            if (arbres[i]->pointsVie <= 0) {
                nb_bois+=10;
                carte->map[arbres[i]->x][arbres[i]->y].marchable = true;
                carte->map[arbres[i]->x][arbres[i]->y].arbre = NULL;
                destroyArbre(arbres[i]);
                arbres[i] = NULL;

            }
        }
    }
    Item * to_add = creerItem(BOIS);
    to_add->nb = nb_bois;
    return to_add;
}

