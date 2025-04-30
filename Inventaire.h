//
// Created by viola on 22/03/2025.
//

#ifndef INVENTAIRE_H
#define INVENTAIRE_H
#include <allegro5/allegro_font.h>
#include <allegro5/bitmap.h>


typedef enum typeItem {
    BOIS, PIERRE, FRUIT
}TypeItem;

typedef struct item {
    int nb;
    int nbMax;
    TypeItem type;
    ALLEGRO_BITMAP* image;
    ALLEGRO_FONT* font;
    int sx, sy;
}Item;

typedef struct inventaire {
    Item** items;
    int taille;
}Inventaire;

void destroyItem(Item* item);
void destroyInventaire(Inventaire* inv);
void afficherItem(Item* item,int i);

Inventaire* creerInventaire();
Inventaire* chargerInventaire();
void saveInventaire(Inventaire* inv);

void afficherInventaire(Inventaire* inv);
void ajouterItem(Inventaire* inv, Item* item);
Item* ajouter_n_item(Item* item, int n, TypeItem type);
Item *creerItem(TypeItem type);
#endif //INVENTAIRE_H
