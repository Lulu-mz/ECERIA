//
// Created by viola on 04/02/2025.
//
#include <stdio.h>
#include "Joueur.h"

//quêtes + déplacements
//==> animation : images
//thème : forêt et eau


Joueur* createJoueur() {
    Joueur* joueur = calloc(1,sizeof(Joueur));
    joueur->x1 = (float) WIDTH/2; //faire démarer au milieu
    joueur->y1 = (float) HEIGHT/2; //faire démarer au milieu
    joueur->x2 = joueur->x1 + 24.0f; //taille du personnage
    joueur->y2 = joueur->y1 + 24.0f; //taille du personnage
    joueur->speed = 5.0f; //vitesse de déplacement
    joueur->direction = STATIQUE; //direction par défaut = statique
    joueur->sprite_x = 0.0f;
    joueur->sprite_y = 0.0f;

    ALLEGRO_BITMAP *sprite_sheet = al_load_bitmap("../Assets/Cats/Characters/Character.png");
    if(!sprite_sheet) {
        printf("Erreur de chargement de l'image\n");
        exit(1);
    }
    joueur->image = sprite_sheet;
    return joueur;
}


void destroyJoueur(Joueur* joueur) {
    free(joueur);
    joueur = NULL;
}


void afficherJoueur(Joueur* joueur, int currentFrame) {
    printf("%d\n", joueur->direction);
    switch(joueur->direction) {
        case DROITE :
            joueur->sprite_x = 2.0f;
            joueur->sprite_y = 3.0f;
        break;

        case GAUCHE :
            joueur->sprite_x = 2.0f;
            joueur->sprite_y = 2.0f;
        break;

        case BAS :
            joueur->sprite_x = 2.0f;
            joueur->sprite_y = 0.0f;
        break;

        case HAUT :
            joueur->sprite_x = 2.0f;
            joueur->sprite_y = 1.0f;
        break;

        case STATIQUE :
            if(joueur->sprite_x>0){
            joueur->sprite_x=0.0f;
            }
        break;
    }
    al_draw_bitmap_region(joueur->image, (joueur->sprite_x+currentFrame)*48, joueur->sprite_y*48, 48, 48, joueur->x1, joueur->y1, 0);
}


void deplacerJoueur(Joueur* joueur) {
    switch (joueur->direction) {
        case DROITE :
            if(joueur->x2 < WIDTH) {
                joueur->x1 += joueur->speed;
                joueur->x2 += joueur->speed;
            }
        break;

        case GAUCHE :
            if (joueur->x1 > 0.0f) {
                joueur->x1 -= joueur->speed;
                joueur->x2 -= joueur->speed;
            }
        break;

        case HAUT :
            if(joueur->y1 > 0.0f) {
                joueur->y1 -= joueur->speed;
                joueur->y2 -= joueur->speed;
            }
        break;

        case BAS :
            if(joueur->y2 < HEIGHT) {
                joueur->y1 += joueur->speed;
                joueur->y2 += joueur->speed;
            }
        break;

        default : //direction par défaut = statique
            break;
    }
}