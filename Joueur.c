//
// Created by viola on 04/02/2025.
//
#include <stdio.h>
#include "Joueur.h"

//Début de la partie :
void initJoueur(Joueur* j) {
    printf("What is your name ?\n");
    scanf("%s",j->nom);
    char g;
    do {
        printf("Input your genre : (F for Female and M for Male) \n");
        scanf(" %c",&g);
        if(g == 'F'|| g == 'M') {
            j->genre = g;
            break;
        }
        printf("Error \n");
    }while(1);
    // Initialisation des items à 0
    for (int i = 0; i < 15; i++) {
        j->items.bois[i] = 0;
        j->items.pierre[i] = 0;
        j->items.eau[i] = 0;
        j->items.poisson[i] = 0;
        j->items.fruit[i] = 0;
    }
}

int compterItems(Joueur *j) {
    int total = 0;
    for (int i = 0; i < 15; i++) {
        total += j->items.bois[i];
        total += j->items.pierre[i];
        total += j->items.eau[i];
        total += j->items.poisson[i];
        total += j->items.fruit[i];
    }
    return total;
}

//quêtes + déplacements
//==> animation : images
//thème : forêt et eau