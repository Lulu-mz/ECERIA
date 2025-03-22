//
// Created by viola on 22/03/2025.
//

#include "Inventaire.h"

#include <stdio.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/bitmap_io.h>


Item* creerBois() {
    Item* item = malloc(1*sizeof(Item));
    item->type = BOIS;
    item->nb = 0;
    item->nbMax = 100;
    item->image = al_load_bitmap("../Assets/Cats/Objects/items.png");
    item->font = al_load_ttf_font("../Assets/Arial.ttf", 20, 0);
    return item;
}

void afficherBois(Item* item) {
    char buffer[10];
    sprintf(buffer, "%d", item->nb);
    al_draw_bitmap_region(item->image, 16, 16, 16, 16, 0, 0, 0);
    al_draw_text(item->font, al_map_rgb(255, 255, 255), 10, 10 , 0, buffer);
}

void destroyBois(Item* item) {
    free(item);
    item = NULL;
}