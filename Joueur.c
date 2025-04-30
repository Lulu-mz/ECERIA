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
    joueur->pos_i = MAP_SIZE/2; //position section carte
    joueur->pos_j = MAP_SIZE/2; //position section carte
    joueur->inHouse = -1;

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
    fprintf(joueurFile, "%d %d\n", joueur->pos_i, joueur->pos_j);
    fprintf(joueurFile, "%d\n", joueur->inHouse);

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
    fscanf(joueurFile, "%d %d", &joueur->pos_i, &joueur->pos_j);
    fscanf(joueurFile, "%d", &joueur->inHouse);
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



void action(Joueur *joueur, Carte *carte) {
    int next_x = -1; //pour le joueur
    int next_y = -1; //pour le joueur
    int next_x2 = -1; //si on est entre 2 cases
    int next_y2 = -1; //si on est entre 2 cases
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
        actionGrassLand(carte,joueur, next_x, next_y);
        actionGrassLand(carte,joueur, next_x2, next_y2);
        actionPorte(carte,joueur, next_x, next_y);
        actionPorte(carte,joueur, next_x2, next_y2);
    }
}

void ouvrirPorte(Carte* carte, Joueur* joueur, int id) {
    if(carte->carte_maison[id] == NULL) {
        carte->carte_maison[id] = genererInterieurMaison(carte->hauteur, carte->largeur);
        saveInterieurMaison(carte->carte_maison[id], joueur->pos_i, joueur->pos_j, id);
    }
    else {
        carte->carte_maison[id] = chargerInterieurMaison(carte->hauteur, carte->largeur,joueur->pos_i, joueur->pos_j, id);
    }
    joueur->inHouse = id;
    joueur->x1 = WIDTH/2 + 4;
    joueur->y1 = (carte->hauteur - 6)*TILE_SIZE;
    joueur->x2 = joueur->x1 + joueur->l; //taille du personnage
    joueur->y2 = joueur->y1 + joueur->h; //taille du personnage
}

void actionGrassLand(Carte* carte,Joueur* joueur, int x, int y) {
    if(carte->map[y][x].grassLand != NULL) { //en x2 et en y2 du perso
        if(carte->map[y][x].grassLand->type == ARBRE_F_G && carte->map[y][x+1].grassLand != NULL) {
            Item *i = taperGrassLand(carte, joueur, x+1, y);
            ajouterItem(joueur->inventaire, i);
            destroyItem(i);
        }
        else if(carte->map[y][x].grassLand->type == ARBRE_F_D && carte->map[y][x-1].grassLand != NULL) {
            Item *i = taperGrassLand(carte, joueur, x-1, y);
            ajouterItem(joueur->inventaire, i);
            destroyItem(i);
        }
        //si j'ai un autre gl
        Item *i = taperGrassLand(carte, joueur, x, y);
        ajouterItem(joueur->inventaire, i);
        destroyItem(i);
    }
}


void deplacerJoueurDevantMaison(Carte* carte, Joueur* joueur, int id_m) {
    int nbM = 0;
    for(int i = 0; i < carte->hauteur; i++) {
        for(int j = 0; j < carte->largeur; j++) {
            if(carte->map[i][j].maison != NULL) {
                if(nbM == id_m) {
                    if(i < carte->hauteur) {
                        Porte* porte = creerPorte(carte->map[i][j].maison->type);
                        joueur->x1 = (j + porte->x) * TILE_SIZE + 4;
                        joueur->y1 = (i + porte->y + 1) * TILE_SIZE;
                    }
                    else {
                        joueur->x1 = WIDTH/2 + 4;
                        joueur->y1 = HEIGHT/2 + 4;
                    }
                    joueur->x2 = joueur->x1 + joueur->l; //taille du personnage
                    joueur->y2 = joueur->y1 + joueur->h; //taille du personnage
                }
                nbM++;
            }
        }
    }
}

void actionPorte(Carte* carte, Joueur* joueur, int x, int y) {
    if(carte->map[y][x].porte != NULL) {
        int y_maison = y-(carte->map[y][x].porte->y); //porte simple ou porte gauche
        int x_maison = x-(carte->map[y][x].porte->x); //porte simple ou porte gauche
        if(carte->map[y][x].porte->doubles == true && carte->map[y][x-1].porte != NULL) {
            x_maison -=1;
        } //si y'a une porte à gauche et que c'est double ==> c'est une porte de droite
        ouvrirPorte(carte, joueur, carte->map[y_maison][x_maison].maison->id);
    }
}