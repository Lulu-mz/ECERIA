//
// Created by viola on 20/03/2025.
//

#include "Biome.h"

#include <allegro5/bitmap_draw.h>
#include <allegro5/bitmap_io.h>

#include "Constantes.h"

#include <stdlib.h>


GrassLand* creerGrassLand(int x, int y, GrassLandType type) {
    GrassLand* grass_land = malloc(1 * sizeof(GrassLand));
    grass_land->image = al_load_bitmap("../Assets/Cats/Objects/biome.png");
    grass_land->pointsVie = 100;
    grass_land->x = x;
    grass_land->y = y;
    grass_land->type = type;
    switch(grass_land->type) {
        case ARBRE :
            grass_land->sx = 0;
            grass_land->sy = 0;
            grass_land->h = 32;
            grass_land->w = 16;
            break;
        case ROCHER :
            grass_land->sx = 128;
            grass_land->sy = 16;
            grass_land->h = 16;
            grass_land->w = 16;
            break;
        // case ARBRE_F :
        //     grass_land->sx = 52;
        //     grass_land->sy = 0;
        //     grass_land->h = 30;
        //     grass_land->w = 23;
    }

    return grass_land;
}

void destroyGrassLand(GrassLand* gl) {
    free(gl);
}

void afficherGrassLand(GrassLand *gL, int pos_x, int pos_y) {
    float x = 0;
    float y = 0;
    switch (gL->type) {
        case ARBRE :
            x = pos_x * TILE_SIZE;
            y = (pos_y * TILE_SIZE) - TILE_SIZE;
            break;
        case ROCHER :
            x = pos_x * TILE_SIZE;
            y = pos_y * TILE_SIZE;
            break;
        // case ARBRE_F :
        //     x = pos_x * TILE_SIZE;
        //     y = pos_y * TILE_SIZE;
        // break;
    }
    // al_draw_bitmap_region(gL->image, gL->sx, gL->sy, gL->w, gL->h, x, y, 0);
    al_draw_scaled_bitmap(gL->image, gL->sx, gL->sy, gL->w, gL->h, x, y, gL->w*2, gL->h*2,0);
}