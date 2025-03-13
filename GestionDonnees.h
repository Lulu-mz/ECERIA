//
// Created by viola on 05/02/2025.
//

#ifndef GESTIONDONNEES_H
#define GESTIONDONNEES_H
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "Joueur.h"

void enregistrerJoueurDansCSV(Joueur *j);
int joueurEstInscrit(const char *nomJoueur);
void mettreAJourJoueurDansCSV(Joueur *j);

#endif //GESTIONDONNEES_H
