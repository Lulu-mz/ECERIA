//
// Created by viola on 22/03/2025.
//

#ifndef INVENTAIRE_H
#define INVENTAIRE_H
#include <allegro5/allegro_font.h>
#include <allegro5/bitmap.h>


typedef enum typeItem {
    BOIS,
}TypeItem;

typedef struct item {
    int nb;
    int nbMax;
    TypeItem type;
    ALLEGRO_BITMAP* image;
    ALLEGRO_FONT* font;
}Item;

typedef struct inventaire {
    Item** items;
    int taille;
}Inventaire;

Item* creerBois();
void afficherBois(Item* item);

#endif //INVENTAIRE_H
