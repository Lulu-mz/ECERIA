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
    c.pnj = NULL;

    if (valeur == 0) {
        c.grassLand = creerGrassLand(i, j, ARBRE);
    } else if (valeur == 1) {
        c.grassLand = creerGrassLand(i, j, ROCHER);
    } else if (valeur == 2) {
        c.grassLand = creerGrassLand(i, j, ARBRE_F_G);
    } else if (valeur == 6) {
        c.grassLand = creerGrassLand(i, j, ARBRE_F_D);
    } else if (valeur == 99) {
        c.vide = true;
    } else if (valeur == 3) {
        c.maison = creerMaison(MAISON_0);
    } else if (valeur == 4) {
        c.maison = creerMaison(MAISON_1);
    } else if (valeur == 10) {
        c.porte = creerPorte(MAISON_0);
    } else if (valeur == 11) {
        c.porte = creerPorte(MAISON_1);
    }
    else if (valeur == 20) {
        c.pnj = creerPnj(VACHE);
    }
    else if (valeur == 21) {
        c.pnj = creerPnj(POULE);
    }
    else if (valeur == 22) {
        c.pnj = creerPnj(PERSO_0);
    }
    else {
        c.marchable = true;
    }
    return c;
}

Carte *chargerCarte(int mapWidth, int mapHeight, int pos_i, int pos_j) {
    al_init_image_addon();

    char buffer[20];
    sprintf(buffer, "../Save/map_%d_%d.txt", pos_i, pos_j);
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
    ALLEGRO_BITMAP *lac = al_load_bitmap("../Assets/Cats/Tilesets/Water.png");
    if (!herbe || !herbe2 || !fleur || !lac) {
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
            carte->map[i][j].sx = 0;
            carte->map[i][j].sy = 0;
            carte->map[i][j].pnj = NULL;
            carte->map[i][j].porte = NULL;
            carte->map[i][j].maison = NULL;

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
                case 5:
                    carte->map[i][j].typeCase = EAU;
                    carte->map[i][j].image = lac;
                    carte->map[i][j].marchable = false;
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

    chargerBiome(carte, pos_i, pos_j);
    carte->nbMaison = compterMaison(carte, pos_i, pos_j);
    attribuateIDHouse(carte);
    carte->carte_maison = malloc(carte->nbMaison * sizeof(Carte *));

    return carte;
}


Carte *creerCarte(int w, int h) {
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
            int valeur = rand() % 3; //génère 0, 1, ou 2

            carte->map[i][j].grassLand = NULL;
            carte->map[i][j].size = TILE_SIZE;
            carte->map[i][j].marchable = true;
            carte->map[i][j].maison = NULL;
            carte->map[i][j].vide = false;
            carte->map[i][j].porte = NULL;
            carte->map[i][j].sx = 0;
            carte->map[i][j].sy = 0;
            carte->map[i][j].pnj = NULL;

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
    carte->nbMaison = genererMaisons(carte);
    attribuateIDHouse(carte);
    carte->carte_maison = malloc(carte->nbMaison * sizeof(Carte *));
    for (int k = 0; k < carte->nbMaison; k++) {
        carte->carte_maison[k] = NULL;
    }
    genererLac(carte);
    genererGrassLand(carte);
    genererPnj(carte, 6);
    return carte;
}


int saveCarte(Carte *carte, int pos_i, int pos_j) {
    char buffer[20];
    sprintf(buffer, "../Save/map_%d_%d.txt", pos_i, pos_j);
    FILE *mapFile = fopen(buffer, "w");
    sprintf(buffer, "../Save/biome_%d_%d.txt", pos_i, pos_j);
    FILE *grassLandFile = fopen(buffer, "w");
    if (mapFile == NULL || grassLandFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    for (int i = 0; i < carte->hauteur; i++) {
        for (int j = 0; j < carte->largeur; j++) {
            int type = carte->map[i][j].typeCase;
            fprintf(mapFile, "%d", type);

            int glType = -1; //il n'y a pas de grassLand
            if (carte->map[i][j].grassLand != NULL) {
                //égal à null par défaut => pas de grassLand
                glType = carte->map[i][j].grassLand->valeur;
            } else if (carte->map[i][j].maison != NULL) {
                glType = carte->map[i][j].maison->valeur; //soit 3 soit 4 : début pour le type de maison
            } else if (carte->map[i][j].porte != NULL) {
                glType = carte->map[i][j].porte->val; //soit 10 soit 11 : pour porte double ou simple
            }
            else if (carte->map[i][j].pnj != NULL) {
                glType = carte->map[i][j].pnj->val;
            }
            else if (carte->map[i][j].vide == true) {
                glType = 99;
            }
            fprintf(grassLandFile, "%d", glType);

            if (j < carte->largeur - 1) {
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


void afficherCarte(Carte *carte, int currentFrame) {
    for (int i = 0; i < carte->hauteur; i++) {
        for (int j = 0; j < carte->largeur; j++) {
            int imageL = 16, imageH = 16;
            if (carte->map[i][j].vide == false && carte->map[i][j].porte == NULL) {
                //al_draw_rectangle(j * 32, i * 32, j * 32 + 32, i * 32 + 32, al_map_rgb(0, 0, 0), 2);
                al_draw_scaled_bitmap(carte->map[i][j].image, carte->map[i][j].sx, carte->map[i][j].sy, imageL, imageH,
                                      j * carte->map[i][j].size, i * carte->map[i][j].size, carte->map[i][j].size,
                                      carte->map[i][j].size, 0);
            }
            if (carte->map[i][j].grassLand != NULL) {
                afficherGrassLand(carte->map[i][j].grassLand, j, i); //Inversé
            }
            if (carte->map[i][j].maison != NULL) {
                afficherMaison(carte->map[i][j].maison, j, i);
            }
            if (carte->map[i][j].pnj != NULL) {
                afficherPnj(carte->map[i][j].pnj, j, i, currentFrame);
            }
        }
    }
}


void destroyCarte(Carte *carte) {
    free(carte->carte_maison);
    carte->carte_maison = NULL;
    for (int i = 0; i < carte->hauteur; i++) {
        free(carte->map[i]);
    }

    free(carte->map);
    free(carte);
    carte = NULL;
}

int compterMaison(Carte *carte, int pos_i, int pos_j) {
    int nbM = 0;
    char buffer[20];
    sprintf(buffer, "../Save/biome_%d_%d.txt", pos_i, pos_j);
    FILE *fichier = fopen(buffer, "r");
    if (!fichier) {
        perror("Erreur ouverture fichier biome.txt");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < carte->hauteur; i++) {
        for (int j = 0; j < carte->largeur; j++) {
            int valeur = -1;
            fscanf(fichier, "%d", &valeur);
            if (valeur == 3 || valeur == 4) {
                nbM++;
            }
        }
    }
    return nbM;
}

void chargerBiome(Carte *carte, int pos_i, int pos_j) {
    char buffer[20];
    sprintf(buffer, "../Save/biome_%d_%d.txt", pos_i, pos_j);
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
            carte->map[i][j] = chargerCase(valeur, carte->map[i][j], i, j);
        }
    }

    fclose(fichier);
}


Item *taperGrassLand(Carte *carte, Joueur *joueur, int x, int y) {
    GrassLand *grass_land = carte->map[y][x].grassLand;
    int nb_items = 0;
    TypeItem type_item = BOIS;

    if (grass_land != NULL) {
        grass_land->pointsVie -= joueur->degats;
        type_item = grass_land->type;
        if (grass_land->type == ARBRE_F_D) {
            type_item = BOIS;
        }
        nb_items += 5;
        if (grass_land->pointsVie <= 0) {
            nb_items += 10;
            carte->map[grass_land->x][grass_land->y].marchable = true;
            carte->map[grass_land->x][grass_land->y].grassLand = NULL;
            destroyGrassLand(grass_land);
            grass_land = NULL;
        }
    }

    Item *to_add = creerItem(type_item);
    to_add->nb = nb_items;
    return to_add;
}


int isValidSpaceForHouses(Carte *carte, int pos_x, int pos_y, Maison *m) {
    for (int y = pos_y; y < pos_y + m->hauteur; y++) {
        for (int x = pos_x; x < pos_x + m->largeur; x++) {
            //Est ce que c'est libre
            if (isValidSpace(carte, pos_x, pos_y) == 0) {
                // Recommence
                return 0;
            }
        }
    }
    return 1;
}

int isValidSpace(Carte *carte, int x, int y) {
    if (carte->map[y][x].grassLand != NULL || carte->map[y][x].maison != NULL || carte->map[y][x].vide == true || carte
        ->map[y][x].porte != NULL || carte->map[y][x].typeCase == EAU || carte->map[y][x].pnj != NULL) {
        return 0;
    }
    return 1;
}

int genererMaisons(Carte *carte) {
    srand(time(NULL));
    int pos_x, pos_y;
    int nbMaison = rand() % 5;
    for (int i = 0; i < nbMaison; i++) {
        TypeMaison type = rand() % 2;
        Maison *m = creerMaison(type);

        do {
            pos_x = rand() % ((carte->largeur) - (m->largeur));
            pos_y = rand() % ((carte->hauteur) - (m->hauteur));
        } while (!isValidSpaceForHouses(carte, pos_x, pos_y, m));

        Porte *p = creerPorte(type);

        for (int y = pos_y; y < pos_y + m->hauteur; y++) {
            for (int x = pos_x; x < pos_x + m->largeur; x++) {
                if (y == pos_y && x == pos_x) {
                    carte->map[y][x] = chargerCase(m->valeur, carte->map[y][x], y, x);
                } else if ((x == p->x + pos_x && y == p->y + pos_y) || (
                               p->doubles == true && x == p->x + pos_x + 1 && y == p->y + pos_y)) {
                    carte->map[y][x] = chargerCase(p->val, carte->map[y][x], y, x);
                } else {
                    carte->map[y][x] = chargerCase(99, carte->map[y][x], y, x);
                }
            }
        }
        destroyMaison(m);
        destroyPorte(p);
    }
    return nbMaison;
}

void genererLac(Carte *carte) {

    srand(time(NULL));
    int nbLac = rand()%3;
    int i, j;
    for (int k = 0; k < nbLac; k++) {
        do {
            i = rand() % carte->hauteur;
            j = rand() % carte->largeur;
        } while (isValidSpace(carte, j, i) == 0);
        generationLacPropagation(carte,i,j,5);
    }
}

void generationLacPropagation(Carte* carte, int i,int j, int tailleMax){
    ALLEGRO_BITMAP *lac = al_load_bitmap("../Assets/Cats/Tilesets/Water.png");
    if (!lac) {
        printf("Erreur de chargement de l'image\n");
        exit(1);
    }
    if (tailleMax <= 0 || i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT ) return;
    if (isValidSpace(carte,j,i)) {
        carte->map[i][j].typeCase = EAU;
        carte->map[i][j].marchable = false;
        carte->map[i][j].image = lac;
        carte->map[i][j].sx = 0;
        carte->map[i][j].sy = 0;
    }

    // propagation aléatoire
    if (rand() % 2) generationLacPropagation(carte, i + 1, j, tailleMax - 1);
    if (rand() % 2) generationLacPropagation(carte, i - 1, j, tailleMax - 1);
    if (rand() % 2) generationLacPropagation(carte, i, j + 1, tailleMax - 1);
    if (rand() % 2) generationLacPropagation(carte, i, j - 1, tailleMax - 1);
}

void genererGrassLand(Carte *carte) {
    srand(time(NULL));
    int i, j;
    int nbGrassLand = rand() % 20;
    for (int k = 0; k < nbGrassLand; k++) {
        GrassLandType type = rand() % 3; // 0 : arbre ou 1 : rocher ou 2 : arbre fruit
        do {
            i = rand() % carte->hauteur;
            j = rand() % carte->largeur;
        } while (isValidSpace(carte, j, i) == 0 || (type == ARBRE_F_G && isValidSpace(carte, j + 1, i) == 0));
        carte->map[i][j] = chargerCase(type, carte->map[i][j], i, j);
        if (type == ARBRE_F_G) {
            carte->map[i][j + 1] = chargerCase(6, carte->map[i][j + 1], i, j + 1);
        }
    }
}

void attribuateIDHouse(Carte *carte) {
    int nbMaison = 0;
    for (int i = 0; i < carte->hauteur; i++) {
        for (int j = 0; j < carte->largeur; j++) {
            if (carte->map[i][j].maison != NULL) {
                carte->map[i][j].maison->id = nbMaison;
                nbMaison++;
            }
        }
    }
}


void saveInterieurMaison(Carte *carte, int pos_i, int pos_j, int id) {
    char buffer[50];
    sprintf(buffer, "../Save/house_%d_%d_%d.txt", pos_i, pos_j, id);
    FILE *fichier = fopen(buffer, "w");
    for (int i = 0; i < carte->hauteur; i++) {
        for (int j = 0; j < carte->largeur; j++) {
            fprintf(fichier, "%d", carte->map[i][j].typeCase);
            if (j < carte->largeur - 1) {
                fprintf(fichier, " ");
            }
        }
        fprintf(fichier, "\n");
    }
    fclose(fichier);
}


Carte *chargerInterieurMaison(int h, int w, int pos_i, int pos_j, int id) {
    char buffer[50];
    sprintf(buffer, "../Save/house_%d_%d_%d.txt", pos_i, pos_j, id);
    FILE *fichier = fopen(buffer, "r");
    if (!fichier) {
        return NULL;
    }

    Carte *carteM = malloc(1 * sizeof(Carte));
    carteM->map = malloc(h * sizeof(Case *));
    for (int i = 0; i < h; i++) {
        carteM->map[i] = malloc(w * sizeof(Case));
    }

    carteM->largeur = w;
    carteM->hauteur = h;

    ALLEGRO_BITMAP *floor = al_load_bitmap("../Assets/Houses/floor.png");
    ALLEGRO_BITMAP *noir = al_load_bitmap("../Assets/Houses/noir.png");
    if (!floor) {
        printf("Erreur de chargement de l'image\n");
        exit(1);
    }

    for (int i = 0; i < carteM->hauteur; i++) {
        for (int j = 0; j < carteM->largeur; j++) {
            carteM->map[i][j].maison = NULL;
            carteM->map[i][j].grassLand = NULL;
            carteM->map[i][j].vide = false;
            carteM->map[i][j].porte = NULL;
            carteM->map[i][j].pnj = NULL;
            carteM->map[i][j].image = NULL;
            carteM->map[i][j].size = TILE_SIZE;
            carteM->map[i][j].sx = 0;
            carteM->map[i][j].sy = 0;

            int valeur = -1;
            fscanf(fichier, "%d", &valeur);

            if (valeur == 3) {
                carteM->map[i][j].image = floor;
                carteM->map[i][j].sx = 16;
                carteM->map[i][j].sy = 16;
                carteM->map[i][j].typeCase = SOL;
                carteM->map[i][j].marchable = true;
            } else if (valeur == 4) {
                carteM->map[i][j].image = noir;
                carteM->map[i][j].typeCase = NOIR;
                carteM->map[i][j].marchable = false;
            }
        }
    }
    fclose(fichier);
    return carteM;
}


Carte *genererInterieurMaison(int h, int w) {
    srand(time(NULL));
    int marge_w = 0;
    int marge_h = 0;
    do {
        marge_w = rand() % 10;
        marge_h = rand() % 5;
    } while (marge_w == 0 || marge_h == 0);

    Carte *enfant = malloc(1 * sizeof(Carte));
    enfant->map = malloc(h * sizeof(Case *));
    for (int i = 0; i < h; i++) {
        enfant->map[i] = malloc(w * sizeof(Case));
    }

    enfant->largeur = w;
    enfant->hauteur = h;

    ALLEGRO_BITMAP *image = al_load_bitmap("../Assets/Houses/floor.png");
    ALLEGRO_BITMAP *noir = al_load_bitmap("../Assets/Houses/noir.png");
    if (!image || !noir) {
        printf("Erreur de chargement d'image\n");
        exit(1);
    }

    for (int i = 0; i < enfant->hauteur; i++) {
        for (int j = 0; j < enfant->largeur; j++) {
            enfant->map[i][j].maison = NULL;
            enfant->map[i][j].grassLand = NULL;
            enfant->map[i][j].vide = false;
            enfant->map[i][j].porte = NULL;
            enfant->map[i][j].size = TILE_SIZE;
            enfant->map[i][j].marchable = false;
            enfant->map[i][j].image = noir;
            enfant->map[i][j].sx = 0;
            enfant->map[i][j].sy = 0;
            enfant->map[i][j].typeCase = NOIR;
            enfant->map[i][j].pnj = NULL;
        }
    }
    for (int i = marge_h; i < enfant->hauteur - marge_h; i++) {
        for (int j = marge_w; j < enfant->largeur - marge_w; j++) {
            enfant->map[i][j].marchable = true;
            enfant->map[i][j].image = image;
            enfant->map[i][j].sx = 16;
            enfant->map[i][j].sy = 16;
            enfant->map[i][j].typeCase = SOL;
        }
    }
    for (int i = enfant->hauteur - marge_h; i < enfant->hauteur; i++) {
        int j = enfant->largeur / 2;
        enfant->map[i][j].marchable = true;
        enfant->map[i][j].image = image;
        enfant->map[i][j].sx = 16;
        enfant->map[i][j].sy = 16;
        enfant->map[i][j].typeCase = SOL;
    }
    return enfant;
}

void genererPnj(Carte* carte, int maxPnj) {
    srand(time(NULL));
    int i, j;
    int nbPnj = rand() % maxPnj;
    for (int k = 0; k < nbPnj; k++) {
        TypePnj type = rand() % 3;
        do {
            i = rand() % carte->hauteur;
            j = rand() % carte->largeur;
        } while (isValidSpace(carte, j, i) == 0);
        carte->map[i][j] = chargerCase((int)type+20, carte->map[i][j], i, j);
    }
}