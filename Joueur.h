//
// Created by viola on 04/02/2025.
//

#ifndef JOUEUR_H
#define JOUEUR_H

typedef struct F {
    int bois[15]; //fait
    int pierre[15]; //fait //TODO : à vérifier
    int eau[15];
    int poisson[15]; //fait
    int fruit[15];
}Fourniture;

typedef struct J {
    char nom[50];
    char genre; //F for Female and M for Male
    Fourniture items;
}Joueur;

void initJoueur(Joueur* j);
int compterItems(Joueur *j);
#endif //JOUEUR_H
