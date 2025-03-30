//
// Created by viola on 04/03/2025.
//

#include "Carte.h"

#include <assert.h>
#include "Inventaire.h"
#include "Joueur.h"

Carte *chargerCarte(int mapWidth, int mapHeight) {
    al_init_image_addon();

    srand(time(NULL));

    Carte *carte = malloc(1 * sizeof(Carte));

    carte->map = malloc(mapHeight * sizeof(Case *));
    for (int i = 0; i < mapHeight; i++) {
        carte->map[i] = malloc(mapWidth * sizeof(Case));
    }

    ALLEGRO_BITMAP *herbe = al_load_bitmap("../Assets/Tiles/tile_0000.png");
    ALLEGRO_BITMAP *herbe2 = al_load_bitmap("../Assets/Tiles/tile_0001.png");
    ALLEGRO_BITMAP *fleur = al_load_bitmap("../Assets/Tiles/tile_0002.png");
    if (!herbe || !herbe2 || !fleur) {
        printf("Erreur de chargement de l'image\n");
        exit(1);
    }

    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            carte->map[i][j].marchable = true;
            carte->map[i][j].typeCase = HERBE;
            carte->map[i][j].image = herbe;
            carte->map[i][j].grassLand = NULL;
            if (rand() % 15 < 1) {
                //une chance sur 15 = 6,6%
                carte->map[i][j].typeCase = FLEUR;
                carte->map[i][j].image = fleur;
            } else if (rand() % 10 < 2) {
                //une chance sur 10 = 10%
                carte->map[i][j].typeCase = HERBE2;
                carte->map[i][j].image = herbe2;
            }
        } //sinon herbe normale
    }
    carte->largeur = mapWidth;
    carte->hauteur = mapHeight;

    ajouterGrassLand(carte);

    return carte;
}





void afficherCarte(Carte *carte) {
    for (int i = 0; i < carte->hauteur; i++) {
        for (int j = 0; j < carte->largeur; j++) {
            al_draw_bitmap(carte->map[i][j].image, j * 16, i * 16, 0);
            if (carte->map[i][j].grassLand != NULL) {
                afficherGrassLand(carte->map[i][j].grassLand, j, i); //Inversé
            }
        }
    }
}


void destroyCarte(Carte *carte) {
    for (int i = 0; i < carte->hauteur; i++) {
        free(carte->map[i]);
    }
    free(carte->map);
    free(carte);
    carte = NULL;
}



void ajouterGrassLand(Carte* carte) {
    srand(time(NULL));
    GrassLandType type = 0;
    for(int i =0;i<carte->hauteur;i++) {
        for(int j = 0;j<carte->largeur;j++) {
            if (rand() % 50 < 1) {
                type = rand()%2;
                carte->map[i][j].grassLand = creerGrassLand(i,j, type);
                carte->map[i][j].marchable = false;
            }

        }
    }
}

//TODO : call twice instead of array
Item* taperGrassLand(Carte* carte, Joueur* joueur, int x, int y, int x2, int y2) {
    int aimedCase = 1;
    int nb_items = 0;
    TypeItem type_item = BOIS;
    GrassLand* grass_lands[2];
    grass_lands[0] = carte->map[y][x].grassLand;
    if (x!=x2 || y !=y2) {
        grass_lands[1] = carte->map[y2][x2].grassLand;
        aimedCase = 2;
    }
    for(int i = 0; i<aimedCase; i++) {
        if (grass_lands[i]!=NULL) {
            //on regarde si y'a un gl
            grass_lands[i]->pointsVie -= joueur->degats;
            type_item = grass_lands[i]->type;
            //bois (item) = type grass_land: arbre
            //pierre (item) = type grass_land: rocher
            nb_items+=5;
            if (grass_lands[i]->pointsVie <= 0) {
                nb_items+=10;
                carte->map[grass_lands[i]->x][grass_lands[i]->y].marchable = true;
                carte->map[grass_lands[i]->x][grass_lands[i]->y].grassLand = NULL;
                destroyGrassLand(grass_lands[i]);
                grass_lands[i] = NULL;

            }
        }
    }
    Item * to_add = creerItem(type_item);
    to_add->nb = nb_items;
    return to_add;
}