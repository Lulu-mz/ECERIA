//
// Created by viola on 04/02/2025.
//
#include <stdio.h>
#include "Joueur.h"

#include <allegro5/allegro_primitives.h>

#include "Biome.h"


//quêtes + déplacements
//==> animation : images
//thème : forêt et eau


Joueur *createJoueur() {
    Joueur *joueur = calloc(1, sizeof(Joueur));
    joueur->l = 24.0f; //hitbox
    joueur->h = 24.0f; //hitbox
    joueur->x1 = (float) WIDTH / 2+4; //faire démarer au milieu
    joueur->y1 = (float) HEIGHT / 2+4; //faire démarer au milieu
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

void saveJoueur(Joueur* joueur) {
    FILE *joueurFile = fopen("../Save/joueur.txt", "w");
    if (joueurFile == NULL){
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }
    fprintf(joueurFile, "%f %f\n", joueur->x1, joueur->y1);
    fprintf(joueurFile, "%f %f\n", joueur->sprite_x, joueur->sprite_y);
    fprintf(joueurFile, "%d %d\n", joueur->direction, joueur->regard);

    saveInventaire(joueur->inventaire);
}


Joueur* chargerJoueur() {
    FILE *joueurFile = fopen("../Save/joueur.txt", "r");
    if (joueurFile == NULL){
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }
    Joueur* joueur = createJoueur();
    fscanf(joueurFile, "%f %f", &joueur->x1, &joueur->y1);
    fscanf(joueurFile, "%f %f", &joueur->sprite_x, &joueur->sprite_y);
    fscanf(joueurFile, "%d %d", &joueur->direction, &joueur->regard);
    joueur->x2 = joueur->x1 + joueur->l; //taille du personnage
    joueur->y2 = joueur->y1 + joueur->h; //taille du personnage

    joueur->inventaire = chargerInventaire();
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
    al_draw_rectangle(joueur->x1, joueur->y1, joueur->x2, joueur->y2, al_map_rgba(255, 0, 0, 50), 2);//hitbox
    int imageL = 16;
    int imageH = 16;
    al_draw_scaled_bitmap(joueur->image,  (joueur->sprite_x + (currentFrame * 3)) * 16, joueur->sprite_y * 16, imageL,
        imageH,(joueur->x1)-4 , (joueur->y1)-4 , TILE_SIZE, TILE_SIZE, 0);
    // 4 corresponds aux marges pour centrer le personnage (4 + 24 + 4 ) = 32
    afficherInventaire(joueur->inventaire);
}

void deplacerJoueur(Joueur *joueur, Carte *carte) {
    int next_x;
    int next_y;
    switch (joueur->direction) {
        case DROITE:
            next_x = (int) (joueur->x2 + joueur->speed) / TILE_SIZE;
            next_y = (int) joueur->y2 / TILE_SIZE;
            joueur->regard = RIGHT;
            if (joueur->x2 < WIDTH && next_x<WIDTH/TILE_SIZE&& carte->map[next_y][next_x].marchable == true && carte->map[(int) joueur->y1 / TILE_SIZE][
                    next_x].marchable == true) {
                joueur->x1 += joueur->speed;
                joueur->x2 += joueur->speed;
            }
            break;

        case GAUCHE:
            next_x = (int) (joueur->x1 - joueur->speed) / TILE_SIZE;
            next_y = (int) joueur->y1 / TILE_SIZE;
            joueur->regard = LEFT;
            if (joueur->x1 > 0.0f && carte->map[next_y][next_x].marchable == true && carte->map[(int) joueur->y2 / TILE_SIZE][
                    next_x].marchable == true) {
                joueur->x1 -= joueur->speed;
                joueur->x2 -= joueur->speed;
            }
            break;

        case HAUT:
            next_x = (int) joueur->x1 / TILE_SIZE;
            next_y = (int) (joueur->y1 - joueur->speed) / TILE_SIZE;
            joueur->regard = UP;
            if (joueur->y1 > 0.0f && carte->map[next_y][next_x].marchable == true && carte->map[next_y][
                    (int) joueur->x2 / TILE_SIZE].marchable == true) {
                joueur->y1 -= joueur->speed;
                joueur->y2 -= joueur->speed;
            }
            break;

        case BAS:
            next_x = (int) joueur->x2 / TILE_SIZE;
            next_y = (int) (joueur->y2 + joueur->speed) / TILE_SIZE;
            joueur->regard = DOWN;
            if (joueur->y2< HEIGHT && next_y<HEIGHT/TILE_SIZE && carte->map[next_y][next_x].marchable == true && carte->map[next_y][
                    (int) joueur->x1 / TILE_SIZE].marchable == true) {
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
            next_x = (int) (joueur->x2 + joueur->speed) / TILE_SIZE; //on regarde la prochaine case où on va (joueur)
            next_y = (int) joueur->y2 / TILE_SIZE;
            next_y2 = (int) joueur->y1 / TILE_SIZE;
            next_x2 = next_x;
            break;

        case LEFT:
            next_x = (int) (joueur->x1 - joueur->speed) / TILE_SIZE;
            next_y = (int) joueur->y1 / TILE_SIZE;
            next_y2 = (int) joueur->y2 / TILE_SIZE;
            next_x2 = next_x;
            break;

        case UP:
            next_x = (int) joueur->x1 / TILE_SIZE;
            next_y = (int) (joueur->y1 - joueur->speed) / TILE_SIZE;
            next_x2 = (int) joueur->x2 / TILE_SIZE;
            next_y2 = next_y;
            break;

        case DOWN:
            next_x = (int) joueur->x2 / TILE_SIZE;
            next_y = (int) (joueur->y2 + joueur->speed) / TILE_SIZE;
            next_x2 = (int) joueur->x1 / TILE_SIZE;
            next_y2 = next_y;
            break;

        default:
            break;
    }
    if(next_x>=0 && next_x< WIDTH/TILE_SIZE && next_y >=0 && next_y < HEIGHT/TILE_SIZE &&
    next_x2>=0 && next_x2< WIDTH/TILE_SIZE && next_y2 >=0 && next_y2 < HEIGHT/TILE_SIZE
        ) {

        Item *i = taperGrassLand(carte, joueur, next_x, next_y, next_x2, next_y2);
        ajouterItem(joueur->inventaire, i);
        destroyItem(i);
    }
}
