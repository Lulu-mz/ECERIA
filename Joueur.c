//
// Created by viola on 04/02/2025.
//
#include <stdio.h>
#include "Joueur.h"

#include "Objets.h"


//quêtes + déplacements
//==> animation : images
//thème : forêt et eau


Joueur *createJoueur() {
    Joueur *joueur = calloc(1, sizeof(Joueur));
    joueur->x1 = (float) WIDTH / 2; //faire démarer au milieu
    joueur->y1 = (float) HEIGHT / 2; //faire démarer au milieu
    joueur->x2 = joueur->x1 + 24.0f; //taille du personnage
    joueur->y2 = joueur->y1 + 24.0f; //taille du personnage
    joueur->speed = 5.0f; //vitesse de déplacement
    joueur->direction = STATIQUE; //direction par défaut = statique
    joueur->regard = DOWN; //direction du regard au début
    joueur->sprite_x = 0.0f;
    joueur->sprite_y = 0.0f;
    joueur->degats = 100;

    ALLEGRO_BITMAP *sprite_sheet = al_load_bitmap("../Assets/Cats/Characters/Character.png");
    if (!sprite_sheet) {
        printf("Erreur de chargement de l'image\n");
        exit(1);
    }
    joueur->image = sprite_sheet;
    return joueur;
}


void destroyJoueur(Joueur *joueur) {
    free(joueur);
    joueur = NULL;
}


void afficherJoueur(Joueur *joueur, int currentFrame) {
    switch (joueur->direction) {
        case DROITE:
            joueur->sprite_x = 2.0f;
            joueur->sprite_y = 3.0f;
            break;

        case GAUCHE:
            joueur->sprite_x = 2.0f;
            joueur->sprite_y = 2.0f;
            break;

        case BAS:
            joueur->sprite_x = 2.0f;
            joueur->sprite_y = 0.0f;
            break;

        case HAUT:
            joueur->sprite_x = 2.0f;
            joueur->sprite_y = 1.0f;
            break;

        case STATIQUE:
            if (joueur->sprite_x > 0) {
                joueur->sprite_x = 0.0f;
            }
            break;
    }
    al_draw_bitmap_region(joueur->image, (joueur->sprite_x + currentFrame) * 48, joueur->sprite_y * 48, 48, 48,
                          joueur->x1, joueur->y1, 0);
}


void deplacerJoueur(Joueur *joueur, Carte *carte) {
    int next_x;
    int next_y;
    switch (joueur->direction) {
        case DROITE:
            next_x = (int) (joueur->x2 + joueur->speed) / 16;
            next_y = (int) joueur->y2 / 16;
            joueur->regard = RIGHT;
            if (joueur->x2 < WIDTH && carte->map[next_y][next_x].marchable == true) {
                joueur->x1 += joueur->speed;
                joueur->x2 += joueur->speed;
            }
            break;

        case GAUCHE:
            next_x = (int) (joueur->x2 - joueur->speed) / 16;
            next_y = (int) joueur->y2 / 16;
            joueur->regard = LEFT;
            if (joueur->x1 > 0.0f && carte->map[next_y][next_x].marchable == true) {
                joueur->x1 -= joueur->speed;
                joueur->x2 -= joueur->speed;
            }
            break;

        case HAUT:
            next_x = (int) joueur->x2 / 16;
            next_y = (int) (joueur->y2 - joueur->speed) / 16;
            joueur->regard = UP;
            if (joueur->y1 > 0.0f && carte->map[next_y][next_x].marchable == true) {
                joueur->y1 -= joueur->speed;
                joueur->y2 -= joueur->speed;
            }
            break;

        case BAS:
            next_x = (int) joueur->x2 / 16;
            next_y = (int) (joueur->y2 + joueur->speed) / 16;
            joueur->regard = DOWN;
            if (joueur->y2 < HEIGHT && carte->map[next_y][next_x].marchable == true) {
                joueur->y1 += joueur->speed;
                joueur->y2 += joueur->speed;
            }
            break;

        default: //direction par défaut = statique
            break;
    }
}

void action(Joueur *joueur, Carte *carte) {
    int next_x = -1;
    int next_y = -1;
    switch (joueur->regard) {
        case RIGHT:
            next_x = (int) (joueur->x2 + joueur->speed) / 16;
            next_y = (int) joueur->y2 / 16;
            break;

        case LEFT:
            next_x = (int) (joueur->x2 - joueur->speed) / 16;
            next_y = (int) joueur->y2 / 16;
            break;

        case UP:
            next_x = (int) joueur->x2 / 16;
            next_y = (int) (joueur->y2 - joueur->speed) / 16;
            break;

        case DOWN:
            next_x = (int) joueur->x2 / 16;
            next_y = (int) (joueur->y2 + joueur->speed) / 16;
            break;

        default:
            break;

    }
    taperArbre(carte, joueur, next_x, next_y);
}
