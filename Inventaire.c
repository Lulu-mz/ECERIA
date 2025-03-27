//
// Created by viola on 22/03/2025.
//

#include "Inventaire.h"

#include <stdio.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/bitmap_io.h>

#include "Joueur.h"


Item *creerBois() {
    Item *item = malloc(1 * sizeof(Item));
    item->type = BOIS;
    item->nb = 0;
    item->nbMax = 100;
    item->image = al_load_bitmap("../Assets/Cats/Objects/items.png");
    item->font = al_load_ttf_font("../Assets/Arial.ttf", 20, 0);
    return item;
}

void afficherItem(Item *item, int i) {
    char buffer[10];
    sprintf(buffer, "%d", item->nb);
    int decalage = i * 48;
    al_draw_scaled_bitmap(item->image, 16, 16, 16, 16, decalage, 0, 32, 32, 0);
    al_draw_text(item->font, al_map_rgb(0, 0, 0), 12 + decalage, 10, 0, buffer);
    al_draw_text(item->font, al_map_rgb(0, 0, 0), 8 + decalage, 10, 0, buffer);
    al_draw_text(item->font, al_map_rgb(0, 0, 0), 10 + decalage, 12, 0, buffer);
    al_draw_text(item->font, al_map_rgb(0, 0, 0), 10 + decalage, 8, 0, buffer);
    al_draw_text(item->font, al_map_rgb(255, 255, 255), 10 + decalage, 10, 0, buffer);
}

void destroyBois(Item *item) {
    free(item);
    item = NULL;
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
        ajouterBois(inv->items[pos], item->nb);
    } else {
        pos = find(inv);
        if (pos != -1) {
            reste = item->nb - item->nbMax;
            inv->items[pos] = ajouterBois(inv->items[pos], item->nb);
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
            if (inventaire->items[i]->type == BOIS) {
                afficherItem(inventaire->items[i], i);
            }
        }
    }
}

// Quand je récupère du bois :

//  Je recherche SI j'ai déjà du bois dans l'inventaire.
// Si j'ai déjà du bois, alors j'ajoute le nouveau nombre
// Si le bois est plein et qu'il me reste du bois a mettre dans l'inventaire, je continue ma recherche


// Je regarde si j'ai de la place dans l'inventaire
// Si j'ai de la place, j'ajoute le bois
