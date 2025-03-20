//
// Created by viola on 20/03/2025.
//

#include "Objets.h"



Arbre* creerArbre() {
    Arbre* arbre = malloc(1 * sizeof(Arbre));
    arbre->image = al_load_bitmap("../Assets/Cats/Objects/biome.png");
    arbre->pointsVie = 100;
    return arbre;
}

void destroyArbre(Arbre* arbre) {
    free(arbre);
}

void taperArbre(Carte* carte, Joueur* joueur, int x, int y) {
    if (carte->map[y][x].arbre != NULL) {
        carte->map[y][x].arbre->pointsVie -= joueur->degats;
        if (carte->map[y][x].arbre->pointsVie <= 0) {
            destroyArbre(carte->map[y][x].arbre);
            carte->map[y][x].arbre = NULL;
            carte->map[y][x].marchable = true;
        }
    }
}

