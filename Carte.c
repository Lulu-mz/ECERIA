//
// Created by viola on 04/03/2025.
//

#include "Carte.h"

#include <assert.h>
#include <allegro5/allegro_primitives.h>

#include "Inventaire.h"
#include "Joueur.h"

Case chargerCase(int valeur, Case c, int i, int j) {
    c.vide = false;
    if (valeur == 0) {
        c.grassLand = creerGrassLand(i, j, ARBRE);
        c.marchable = false;
        c.maison = NULL;
    } else if (valeur == 1) {
        c.grassLand = creerGrassLand(i, j, ROCHER);
        c.marchable = false;
        c.maison = NULL;
    } else if(valeur == 2) {
        c.grassLand = NULL;
        c.marchable = false;
        c.maison = NULL;
        c.vide = true;
    } else if (valeur == 3) {
        c.grassLand = NULL;
        c.marchable = false;
        c.maison = creerMaison(MAISON_0);
    }
    else if (valeur == 4) {
        c.grassLand = NULL;
        c.marchable = false;
        c.maison = creerMaison(MAISON_1);
    }
    else {
        c.grassLand = NULL;
        c.maison = NULL;
    }
    return c;
}

Carte *chargerCarte(int mapWidth, int mapHeight, int pos_i, int pos_j) {
    al_init_image_addon();

    char buffer[20];
    sprintf(buffer,"../Save/map_%d_%d.txt", pos_i, pos_j);
    FILE *fichier = fopen(buffer, "r");
    if (!fichier) {
        return NULL;
    }

    Carte *carte = malloc(sizeof(Carte));
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
            int valeur = 0;
            if (fscanf(fichier, "%d", &valeur) != 1) {
                printf("Erreur de lecture dans map.txt à la position (%d, %d)\n", i, j);
                fclose(fichier);
                exit(EXIT_FAILURE);
            }

            carte->map[i][j].grassLand = NULL;
            carte->map[i][j].size = TILE_SIZE;
            carte->map[i][j].marchable = true;

            switch (valeur) {
                case 0:
                    carte->map[i][j].typeCase = HERBE;
                    carte->map[i][j].image = herbe;
                    break;
                case 1:
                    carte->map[i][j].typeCase = HERBE2;
                    carte->map[i][j].image = herbe2;
                    break;
                case 2:
                    carte->map[i][j].typeCase = FLEUR;
                    carte->map[i][j].image = fleur;
                    break;
                default:
                    printf("Valeur inconnue %d dans map.txt à la position (%d, %d)\n", valeur, i, j);
                    fclose(fichier);
                    exit(EXIT_FAILURE);
            }
        }
    }

    fclose(fichier);
    carte->largeur = mapWidth;
    carte->hauteur = mapHeight;

    chargerGrassLand(carte, pos_i, pos_j);

    return carte;
}

Carte* creerCarte(int w, int h) {
    Carte *carte = malloc(sizeof(Carte));
    carte->map = malloc(h * sizeof(Case *));
    for (int i = 0; i < h; i++) {
        carte->map[i] = malloc(w * sizeof(Case));
    }

    srand(time(NULL));

    ALLEGRO_BITMAP *herbe = al_load_bitmap("../Assets/Tiles/tile_0000.png");
    ALLEGRO_BITMAP *herbe2 = al_load_bitmap("../Assets/Tiles/tile_0001.png");
    ALLEGRO_BITMAP *fleur = al_load_bitmap("../Assets/Tiles/tile_0002.png");
    if (!herbe || !herbe2 || !fleur) {
        printf("Erreur de chargement de l'image\n");
        exit(1);
    }

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int valeur = rand()%3; //génère 0, 1 ou 2

            carte->map[i][j].grassLand = NULL;
            carte->map[i][j].size = TILE_SIZE;
            carte->map[i][j].marchable = true;

            switch (valeur) {
                case 0:
                    carte->map[i][j].typeCase = HERBE;
                    carte->map[i][j].image = herbe;
                break;
                case 1:
                    carte->map[i][j].typeCase = HERBE2;
                carte->map[i][j].image = herbe2;
                break;
                case 2:
                    carte->map[i][j].typeCase = FLEUR;
                carte->map[i][j].image = fleur;
                break;
                default:
                    printf("Valeur inconnue %d dans map.txt à la position (%d, %d)\n", valeur, i, j);
                exit(EXIT_FAILURE);
            }
            valeur = -1; //par défaut : rien
            // 1 chance sur 50 d'avoir un grassland
            if (rand() % 50 < 1) {
                valeur = rand() % 2; // 0 ou 1
            }
            carte->map[i][j] = chargerCase(valeur,carte->map[i][j], i, j);
        }
    }

    carte->largeur = w;
    carte->hauteur = h;

    return carte;
}


int saveCarte(Carte* carte, int pos_i, int pos_j) {
    char buffer[20];
    sprintf(buffer,"../Save/map_%d_%d.txt", pos_i, pos_j);
    FILE *mapFile = fopen(buffer, "w");
    sprintf(buffer,"../Save/biome_%d_%d.txt", pos_i, pos_j);
    FILE *grassLandFile = fopen(buffer, "w");
    if (mapFile == NULL|| grassLandFile == NULL){
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    for (int i = 0; i < carte->hauteur; i++) {
        for (int j = 0; j < carte->largeur; j++) {
            int type = carte->map[i][j].typeCase;
            fprintf(mapFile, "%d", type);

            int glType = -1; //il n'y a pas de grassLand
            if(carte->map[i][j].grassLand != NULL) { //égal à null par défaut => pas de grassLand
                glType = carte->map[i][j].grassLand->type;
            }
            fprintf(grassLandFile, "%d", glType);

            if (j < carte->largeur - 1){
                fprintf(mapFile, " "); // espace entre les colonnes
                fprintf(grassLandFile, " "); // espace entre les colonnes
                }
        }
        fprintf(mapFile, "\n"); // saut de ligne à la fin de chaque ligne
        fprintf(grassLandFile, "\n"); // saut de ligne à la fin de chaque ligne
    }

    fclose(mapFile);
    fclose(grassLandFile);
    return 0;
}


void afficherCarte(Carte *carte) {
    for (int i = 0; i < carte->hauteur; i++) {
        for (int j = 0; j < carte->largeur; j++) {
            int imageL=16,imageH=16;
            if(carte->map[i][j].vide == false) {
                al_draw_rectangle(j*32,i*32,j*32+32,i*32+32,al_map_rgb(0,0,0),2 );
                al_draw_scaled_bitmap(carte->map[i][j].image,0,0,imageL,imageH, j * carte->map[i][j].size, i * carte->map[i][j].size,carte->map[i][j].size,carte->map[i][j].size, 0);
            }
            if (carte->map[i][j].grassLand != NULL) {
                afficherGrassLand(carte->map[i][j].grassLand, j, i); //Inversé
            }
            if (carte->map[i][j].maison != NULL) {
                afficherMaison(carte->map[i][j].maison, j, i);
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


void chargerGrassLand(Carte* carte, int pos_i, int pos_j) {
    char buffer[20];
    sprintf(buffer,"../Save/biome_%d_%d.txt", pos_i, pos_j);
    FILE *fichier = fopen(buffer, "r");
    if (!fichier) {
        perror("Erreur ouverture fichier biome.txt");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < carte->hauteur; i++) {
        for (int j = 0; j < carte->largeur; j++) {
            int valeur = -1;
            if (fscanf(fichier, "%d", &valeur) != 1) {
                printf("Erreur de lecture biome.txt à (%d, %d)\n", i, j);
                fclose(fichier);
                exit(EXIT_FAILURE);
            }
            carte->map[i][j] = chargerCase(valeur,carte->map[i][j], i, j);
        }
    }

    fclose(fichier);
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
