//
// Created by viola on 04/02/2025.
//
#include <stdio.h>
#include "Joueur.h"

#include <allegro5/allegro_primitives.h>

#include "Objets.h"


//quêtes + déplacements
//==> animation : images
//thème : forêt et eau


Joueur *createJoueur() {
    Joueur *joueur = calloc(1, sizeof(Joueur));
    joueur->l = 12.0f;
    joueur->h = 12.0f;
    joueur->x1 = (float) WIDTH / 2; //faire démarer au milieu
    joueur->y1 = (float) HEIGHT / 2; //faire démarer au milieu
    joueur->x2 = joueur->x1 + joueur->l; //taille du personnage
    joueur->y2 = joueur->y1 + joueur->h; //taille du personnage
    joueur->speed = 5.0f; //vitesse de déplacement
    joueur->direction = STATIQUE; //direction par défaut = statique
    joueur->regard = DOWN; //direction du regard au début
    joueur->sprite_x = 1.0f;
    joueur->sprite_y = 1.0f;
    joueur->degats = 100;
    joueur->inventaire = creerInventaire();

    ALLEGRO_BITMAP *sprite_sheet = al_load_bitmap("../Assets/Cats/Characters/Character.png");
    if (!sprite_sheet) {
        printf("Erreur de chargement de l'image\n");
        exit(1);
    }
    joueur->image = sprite_sheet;
    return joueur;
}

void destroyJoueur(Joueur *joueur) {
    destroyInventaire(joueur->inventaire);
    free(joueur);
    joueur = NULL;
}


void afficherJoueur(Joueur *joueur, int currentFrame) {
    switch (joueur->direction) {
        case DROITE:
            joueur->sprite_x = 7.0f;
            joueur->sprite_y = 10.0f;
            break;

        case GAUCHE:
            joueur->sprite_x = 7.0f;
            joueur->sprite_y = 7.0f;
            break;

        case BAS:
            joueur->sprite_x = 7.0f;
            joueur->sprite_y = 1.0f;
            break;

        case HAUT:
            joueur->sprite_x = 7.0f;
            joueur->sprite_y = 4.0f;
            break;

        case STATIQUE:
            if (joueur->sprite_x > 1) {
                joueur->sprite_x = 1.0f;
            }
            break;
    }
    al_draw_rectangle(joueur->x1, joueur->y1, joueur->x2, joueur->y2, al_map_rgba(255, 0, 0, 50), 2);
    int imageL = 16;
    int imageH = 16;
    al_draw_bitmap_region(joueur->image, (joueur->sprite_x + (currentFrame * 3)) * 16, joueur->sprite_y * 16, imageL,
                          imageH,
                          (joueur->x1) - ((imageL - joueur->l) / 2), (joueur->y1) - ((imageH - joueur->h) / 2), 0);

    afficherInventaire(joueur->inventaire);
}


void deplacerJoueur(Joueur *joueur, Carte *carte) {
    int next_x;
    int next_y;
    switch (joueur->direction) {
        case DROITE:
            next_x = (int) (joueur->x2 + joueur->speed) / 16;
            next_y = (int) joueur->y2 / 16;
            joueur->regard = RIGHT;
            if (joueur->x2 < WIDTH && carte->map[next_y][next_x].marchable == true && carte->map[(int) joueur->y1 / 16][
                    next_x].marchable == true) {
                joueur->x1 += joueur->speed;
                joueur->x2 += joueur->speed;
            }
            break;

        case GAUCHE:
            next_x = (int) (joueur->x1 - joueur->speed) / 16;
            next_y = (int) joueur->y1 / 16;
            joueur->regard = LEFT;
            if (joueur->x1 > 0.0f && carte->map[next_y][next_x].marchable == true && carte->map[(int) joueur->y2 / 16][
                    next_x].marchable == true) {
                joueur->x1 -= joueur->speed;
                joueur->x2 -= joueur->speed;
            }
            break;

        case HAUT:
            next_x = (int) joueur->x1 / 16;
            next_y = (int) (joueur->y1 - joueur->speed) / 16;
            joueur->regard = UP;
            if (joueur->y1 > 0.0f && carte->map[next_y][next_x].marchable == true && carte->map[next_y][
                    (int) joueur->x2 / 16].marchable == true) {
                joueur->y1 -= joueur->speed;
                joueur->y2 -= joueur->speed;
            }
            break;

        case BAS:
            next_x = (int) joueur->x2 / 16;
            next_y = (int) (joueur->y2 + joueur->speed) / 16;
            joueur->regard = DOWN;
            if (joueur->y2 < HEIGHT && carte->map[next_y][next_x].marchable == true && carte->map[next_y][
                    (int) joueur->x1 / 16].marchable == true) {
                joueur->y1 += joueur->speed;
                joueur->y2 += joueur->speed;
            }
            break;

        default: //direction par défaut = statique
            break;
    }
}

void action(Joueur *joueur, Carte *carte) {
    int next_x = -1; //pour le joueur
    int next_y = -1; //pour le joueur
    int next_x2 = -1; //pour l'arbre
    int next_y2 = -1; //pour l'arbre
    switch (joueur->regard) {
        case RIGHT:
            next_x = (int) (joueur->x2 + joueur->speed) / 16; //on regarde la prochaine case où on va (joueur)
            next_y = (int) joueur->y2 / 16;
            next_y2 = (int) joueur->y1 / 16;
            next_x2 = next_x;
            break;

        case LEFT:
            next_x = (int) (joueur->x1 - joueur->speed) / 16;
            next_y = (int) joueur->y1 / 16;
            next_y2 = (int) joueur->y2 / 16;
            next_x2 = next_x;
            break;

        case UP:
            next_x = (int) joueur->x1 / 16;
            next_y = (int) (joueur->y1 - joueur->speed) / 16;
            next_x2 = (int) joueur->x2 / 16;
            next_y2 = next_y;
            break;

        case DOWN:
            next_x = (int) joueur->x2 / 16;
            next_y = (int) (joueur->y2 + joueur->speed) / 16;
            next_x2 = (int) joueur->x1 / 16;
            next_y2 = next_y;
            break;

        default:
            break;
    }
    Item *i = taperArbre(carte, joueur, next_x, next_y, next_x2, next_y2);
    ajouterItem(joueur->inventaire, i);
    destroyItem(i);
}
