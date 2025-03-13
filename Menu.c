//
// Created by viola on 05/02/2025.
//
#include "Jeu.h"
#include "Joueur.h"
#include "Menu.h"
#include <stdio.h>

void menu(Joueur * j) {
    int choix = 0;
    printf("Que voulez-vous faire ? \n"
           "1. Nouvelle Partie \n"
           "2. Chercher une quete a faire \n"
           "3. Rien \n");
    scanf("%d", &choix);

    switch(choix) {
        case 1 :
            nouvellePartie(j);
            break;

        case 2 :
            chercherQuete(j);
            break;

        case 3 :
            printf("APPUYER SUR LA TOUCHE ENTREE POUR QUITTER\n");
            fflush(stdin);
            getchar();
            break;

        default:
            printf("Error \n");
            break;
    }
}