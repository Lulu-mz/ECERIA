//
// Created by viola on 04/03/2025.
//

#include "Carte.h"

Carte* chargerCarte(int mapWidth, int mapHeight) {
    al_init_image_addon();

    srand(time(NULL));

    Carte* carte = malloc(1*sizeof(Carte));

    carte->map = malloc(mapHeight*sizeof(Case*));
    for(int i = 0; i < mapHeight; i++) {
        carte->map[i] = malloc(mapWidth*sizeof(Case));
    }

    ALLEGRO_BITMAP* herbe = al_load_bitmap("../Assets/Tiles/tile_0000.png");
    ALLEGRO_BITMAP* herbe2 = al_load_bitmap("../Assets/Tiles/tile_0001.png");
    ALLEGRO_BITMAP* fleur = al_load_bitmap("../Assets/Tiles/tile_0002.png");
    if(!herbe || !herbe2 || !fleur) {
        printf("Erreur de chargement de l'image\n");
        exit(1);
    }

    for(int i = 0; i< mapHeight; i++) {
        for(int j = 0; j<mapWidth; j++) {
            carte->map[i][j].marchable = true;
            carte->map[i][j].typeCase = HERBE;
            carte->map[i][j].image = herbe;
            if(rand()%15 < 1) { //une chance sur 15 = 6,6%
                carte->map[i][j].typeCase = FLEUR;
                carte->map[i][j].image = fleur;
            }
            else if(rand()%10 < 2) { //une chance sur 10 = 10%
                carte->map[i][j].typeCase = HERBE2;
                carte->map[i][j].image = herbe2;
            }
        } //sinon herbe normale
    }
    carte->largeur = mapWidth;
    carte->hauteur = mapHeight;

    return carte;
}

void afficherCarte(Carte* carte) {
    for(int i = 0; i < carte->hauteur; i++) {
        for(int j = 0; j < carte->largeur; j++) {
            al_draw_bitmap(carte->map[i][j].image, j*16, i*16, 0);
        }
    }

}


void destroyCarte(Carte* carte) {
    for(int i = 0; i < carte->hauteur; i++) {
        free(carte->map[i]);
    }
    free(carte->map);
    free(carte);
    carte = NULL;
}