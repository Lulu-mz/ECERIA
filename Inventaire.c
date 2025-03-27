//
// Created by viola on 22/03/2025.
//

#include "Inventaire.h"

#include <stdio.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/bitmap_io.h>

#include "Joueur.h"

Item *creerItem(TypeItem type) {
    Item *item = malloc(1 * sizeof(Item));
    item->type = type;
    item->nb = 0;
    item->nbMax = 100;
    item->image = al_load_bitmap("../Assets/Cats/Objects/items.png");
    item->font = al_load_ttf_font("../Assets/Arial.ttf", 20, 0);
    switch(item->type) {
        case BOIS :
            item->sx = 16;
            item->sy = 16;
        break;

        case PIERRE :
            item->sx = 0;
            item->sy = 16;
        break;
    }
    return item;
}

void afficherItem(Item *item, int i) {
    char buffer[10];
    sprintf(buffer, "%d", item->nb);
    int decalage = i * 48;
    al_draw_scaled_bitmap(item->image, item->sx, item->sy, 16, 16, decalage, 0, 32, 32, 0);
    al_draw_text(item->font, al_map_rgb(0, 0, 0), 12 + decalage, 10, 0, buffer);
    al_draw_text(item->font, al_map_rgb(0, 0, 0), 8 + decalage, 10, 0, buffer);
    al_draw_text(item->font, al_map_rgb(0, 0, 0), 10 + decalage, 12, 0, buffer);
    al_draw_text(item->font, al_map_rgb(0, 0, 0), 10 + decalage, 8, 0, buffer);
    al_draw_text(item->font, al_map_rgb(255, 255, 255), 10 + decalage, 10, 0, buffer);
}

void destroyItem(Item *item) {
    free(item);
    item = NULL;
}

Item* ajouter_n_item(Item *item, int n, TypeItem type) {
    if (item == NULL) {
        item = creerItem(type);
    }
    if (item->nbMax - item->nb >= n) {
        item->nb += n;
    } else if (item->nbMax - item->nb < n) {
        item->nb = item->nbMax;
    }
    return item;
}

int find(Inventaire *inventaire) {
    for (int i = 0; i < inventaire->taille; i++) {
        if (inventaire->items[i] == NULL) {
            return i;
        }
    }
    return -1;
}

int findItem(Inventaire *inv, TypeItem type) {
    for (int i = 0; i < inv->taille; i++) {
        if (inv->items[i] != NULL && inv->items[i]->type == type && inv->items[i]->nb < inv->items[i]->nbMax) {
            return i;
        }
    }
    return -1;
}

Inventaire *creerInventaire() {
    Inventaire *inv = malloc(1 * sizeof(Inventaire));
    inv->taille = 10;
    inv->items = malloc((inv->taille) * sizeof(Item *));
    for (int i = 0; i < inv->taille; i++) {
        inv->items[i] = NULL;
    }
    return inv;
}

void ajouterItem(Inventaire *inv, Item *item) {
    int pos = findItem(inv, item->type);
    int reste = 0;
    if (pos != -1) {
        reste = inv->items[pos]->nb + item->nb - item->nbMax;
        ajouter_n_item(inv->items[pos], item->nb, item->type);
    } else {
        pos = find(inv);
        if (pos != -1) {
            reste = item->nb - item->nbMax;
            inv->items[pos] = ajouter_n_item(inv->items[pos], item->nb, item->type);
        }
    }

    if (reste > 0) {
        item->nb = reste;
        ajouterItem(inv, item);
    }
}

void afficherInventaire(Inventaire *inventaire) {
    for (int i = 0; i < inventaire->taille; i++) {
        if (inventaire->items[i] != NULL) {
                afficherItem(inventaire->items[i], i);
        }
    }
}

//TODO : à corriger destroyInventaire()
void destroyInventaire(Inventaire* inv) {
    for(int i = 0; i< inv->taille; i++) {
        destroyItem(inv->items[i]);
    }
    free(inv);
    inv = NULL;
}

