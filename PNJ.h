//
// Created by viola on 07/05/2025.
//

#ifndef PNJ_H
#define PNJ_H
#include <allegro5/allegro_audio.h>
#include <allegro5/bitmap.h>

#include "Inventaire.h"

#define BUTTON_WIDTH 150
#define BUTTON_HEIGHT 40

typedef enum { TRADE_REFUSE, TRADE_ACCEPT } TradeResult;

typedef enum type_pnj {
    VACHE,
    POULE,
    PERSO_0
}TypePnj;

typedef struct pnj {
    ALLEGRO_BITMAP* image;
    int sx;
    int sy;
    int sw;
    int sh;
    ALLEGRO_SAMPLE* audio;
    TypePnj type;
    int val;
    int nbItems;
    Item* tradeIn;
    Item* tradeOut;
}Pnj;

Pnj* creerPnj(TypePnj type);
Pnj* chargerPnj();
void destroyPnj(Pnj* pnj);
void afficherPnj(Pnj* pnj, int x, int y, int currentFrame);
TradeResult afficherMenuTrade(Pnj* pnj, ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* queue);
#endif //PNJ_H
