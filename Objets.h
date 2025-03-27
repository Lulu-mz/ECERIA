//
// Created by viola on 20/03/2025.
//

#ifndef OBJETS_H
#define OBJETS_H
#include "Carte.h"
#include "Joueur.h"

Arbre* creerArbre(int x, int y);

Item* taperArbre(Carte* carte, Joueur* joueur, int x, int y, int x2, int y2);

#endif //OBJETS_H
