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
    c.maison = NULL;
    c.grassLand = NULL;
    c.porte = NULL;
    c.marchable = false;

    if (valeur == 0) {
        c.grassLand = creerGrassLand(i, j, ARBRE);
    } else if (valeur == 1) {
        c.grassLand = creerGrassLand(i, j, ROCHER);
    } else if(valeur == 2) {
        c.vide = true;
    } else if (valeur == 3) {
        c.maison = creerMaison(MAISON_0);
    }
    else if (valeur == 4) {
        c.maison = creerMaison(MAISON_1);
    }
    else if (valeur == 10) {
        c.porte = creerPorte(MAISON_0);
    }
    else if (valeur == 11) {
        c.porte = creerPorte(MAISON_1);
    }
    else {
        c.marchable = true;
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

    carte->largeur = w;
    carte->hauteur = h;

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
            carte->map[i][j].maison = NULL;
            carte->map[i][j].vide = false;
            carte->map[i][j].porte = NULL;

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
        }
    }
    genererMaisons(carte);
    genererGrassLand(carte);
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
            else if(carte->map[i][j].maison != NULL) {
                glType = carte->map[i][j].maison->valeur; //soit 3 soit 4 : début pour le type de maison
            }
            else if(carte->map[i][j].porte != NULL) {
                glType = carte->map[i][j].porte->val; //soit 10 soit 11 : pour porte double ou simple
            }
            else if(carte->map[i][j].vide == true) {
                glType = 2;
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
            if(carte->map[i][j].vide == false && carte->map[i][j].porte == NULL) {
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


int isValidSpace(Carte* carte, int pos_x, int pos_y, Maison* m){
    for (int y = pos_y ; y < pos_y+m->hauteur ; y++)
    {
        for (int x = pos_x ; x < pos_x+m->largeur ; x++)
        {
            //Est ce que c'est libre
            if (carte->map[y][x].grassLand != NULL || carte->map[y][x].maison != NULL || carte->map[y][x].vide == true || carte->map[y][x].porte != NULL) {
                // Recommence
                return 0;
            }
        }
    }
    return 1;
}

void genererMaisons(Carte* carte) {
    srand(time(NULL));
    int pos_x, pos_y;
    int nbMaison = rand()%5;
    for(int i = 0; i < nbMaison; i++) {
        TypeMaison type = rand()%2;
        Maison * m = creerMaison(type);

        do {
            pos_x = rand()%((carte->largeur)-(m->largeur));
            pos_y = rand()%((carte->hauteur)-(m->hauteur));
        }while(!isValidSpace(carte, pos_x, pos_y, m));

        Porte* p = creerPorte(type);

        for (int y = pos_y ; y < pos_y+m->hauteur ; y++)
        {
            for (int x = pos_x ; x < pos_x+m->largeur ; x++) {
                if(y == pos_y && x == pos_x) {
                    carte->map[y][x] = chargerCase(m->valeur, carte->map[y][x], y, x);
                }
                else if ((x == p->x + pos_x && y == p->y + pos_y ) || (p->doubles == true && x == p->x + pos_x+1 && y == p->y + pos_y) ){
                    carte->map[y][x] = chargerCase(p->val, carte->map[y][x], y, x);
                }
                else {
                    carte->map[y][x] = chargerCase(2, carte->map[y][x], y, x);
                }
            }
        }
        destroyMaison(m);
        destroyPorte(p);
    }
}

void genererGrassLand(Carte* carte) {
    srand(time(NULL));
    int i, j;
    int nbGrassLand = rand()%20;
    for(int k = 0; k<nbGrassLand; k++){
        do {
            i = rand()%carte->hauteur;
            j = rand()%carte->largeur;
        }while(carte->map[i][j].grassLand != NULL || carte->map[i][j].maison != NULL || carte->map[i][j].vide == true || carte->map[i][j].porte != NULL);
        GrassLandType type = rand() % 2; // 0 : arbre ou 1 : rocher
        carte->map[i][j] = chargerCase(type,carte->map[i][j], i, j);
    }
}