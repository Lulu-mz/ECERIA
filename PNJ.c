//
// Created by viola on 07/05/2025.
//
#include "Constantes.h"
#include "PNJ.h"

#include <stdio.h>
#include <allegro5/allegro_primitives.h>


Pnj* creerPnj(TypePnj type) {
    Pnj* pnj = malloc(1*sizeof(Pnj));
    pnj->type = type;
    pnj->val = 20+(int)pnj->type;
    pnj->nbItems = 1;
    pnj->tradeIn = NULL;
    pnj->tradeOut = NULL;
    switch (type) {
        case VACHE :
            pnj->image = al_load_bitmap("../Assets/Cats/Characters/Vache.png");
            pnj->sx = 0;
            pnj->sy = 0;
            pnj->sw = 32;
            pnj->sh = 32;
            pnj->audio = al_load_sample("../Sounds/NPC/cow.wav");
            pnj->tradeIn = creerItem(BOIS);
            pnj->tradeOut = creerItem(PIOCHE);
            pnj->tradeIn->nb = 15;
            pnj->tradeOut->nb = 1;
        break;
        case POULE :
            pnj->image = al_load_bitmap("../Assets/Cats/Characters/Poule.png");
            pnj->sx = 0;
            pnj->sy = 0;
            pnj->sw = 16;
            pnj->sh = 16;
            pnj->audio = al_load_sample("../Sounds/NPC/chicken.wav");
            pnj->tradeIn = creerItem(FRUIT);
            pnj->tradeOut = creerItem(BOIS);
            pnj->tradeIn->nb = 15;
            pnj->tradeOut->nb = 30;
        break;
        case PERSO_0 :
            pnj->image = al_load_bitmap("../Assets/Cats/Characters/Humans/NPC.png");
            pnj->sx = 8;
            pnj->sy = 4;
            pnj->sw = 16;
            pnj->sh = 24;
            pnj->audio = al_load_sample("../Sounds/NPC/hey.wav");
            pnj->tradeIn = creerItem(PIERRE);
            pnj->tradeOut = creerItem(HACHE);
            pnj->tradeIn->nb = 15;
            pnj->tradeOut->nb = 1;
            break;
        default :
        break;
    }
    return pnj;
}

void destroyPnj(Pnj* pnj) {
    free(pnj);
    pnj = NULL;
}

void afficherPnj(Pnj* pnj, int x, int y, int currentFrame) {
    int max = 2;
    if(pnj->type == VACHE) {
        max = currentFrame%3;
        al_draw_scaled_bitmap(pnj->image, (pnj->sx + max)*32, pnj->sy*16, pnj->sw, pnj->sh, (x*TILE_SIZE+4)-TILE_SIZE, y*TILE_SIZE-TILE_SIZE, pnj->sw*2, pnj->sh*2,0);
    }
    else if(pnj->type == POULE) {
        max = currentFrame%2;
        al_draw_scaled_bitmap(pnj->image, (pnj->sx + max)*16, pnj->sy*16, pnj->sw, pnj->sh, (x*TILE_SIZE+4), y*TILE_SIZE, pnj->sw*2, pnj->sh*2,0);
    }
    else if(pnj->type == PERSO_0) {
        max = currentFrame%5;
        al_draw_scaled_bitmap(pnj->image, pnj->sx + max*32, pnj->sy, pnj->sw, pnj->sh, x*TILE_SIZE, y*TILE_SIZE - 8, pnj->sw*2, pnj->sh*2,0);
    }
}

TradeResult afficherMenuTrade(Pnj* pnj, ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* queue) {
    bool running = true;
    ALLEGRO_COLOR bgColor = al_map_rgb(30, 30, 30);
    ALLEGRO_COLOR borderColor = al_map_rgb(200, 200, 200);
    ALLEGRO_COLOR textColor = al_map_rgb(255, 255, 255);

    // Positions de base
    int boxX = 100, boxY = 100;
    int boxW = 600, boxH = 400;
    int margin = 20;


    while (running) {
        // Dessin de fond du menu
        al_draw_filled_rectangle(boxX, boxY, boxX + boxW, boxY + boxH, bgColor);
        al_draw_rectangle(boxX, boxY, boxX + boxW, boxY + boxH, borderColor, 2);

        // Titre
        al_draw_text(font, textColor, boxX + boxW / 2, boxY + 10, ALLEGRO_ALIGN_CENTER, "Échange avec le PNJ");

        // Affichage des items demandés
        al_draw_text(font, textColor, boxX + margin, boxY + 50, 0, "Vous devez donner :");
        for (int i = 0; i < pnj->nbItems; i++) {
            if (pnj->tradeIn) {

                Item * item = pnj->tradeIn;
                if(item != NULL)
                {
                    al_draw_scaled_bitmap(item->image, item->sx, item->sy, 16, 16, boxX + margin + i * 60, boxY + 80, 32, 32, 0);
                    char buffer[20];
                    sprintf(buffer, "%d", item->nb);
                    al_draw_text(font, textColor, boxX + margin + i * 60, boxY + 120, 0, buffer );
                }

            }
        }

        // Affichage des items donnés en échange
        al_draw_text(font, textColor, boxX + margin, boxY + 160, 0, "En échange, vous recevrez :");
        for (int i = 0; i < pnj->nbItems; i++) {
            if (pnj->tradeOut) {
                Item * item = pnj->tradeOut;
                if(item != NULL)
                {
                    al_draw_scaled_bitmap(item->image, item->sx, item->sy, 16, 16, boxX + margin + i * 60, boxY + 190, 32, 32, 0);
                    char buffer[20];
                    sprintf(buffer, "%d", item->nb);
                    al_draw_text(font, textColor, boxX + margin + i * 60, boxY + 230, 0, buffer );                }
            }
        }

        int b1X = boxX + margin;
        int b1Y = boxY + boxH - BUTTON_HEIGHT - margin;
        al_draw_filled_rectangle(b1X,b1Y,b1X+BUTTON_WIDTH,b1Y+BUTTON_HEIGHT, al_map_rgb(0, 200, 0));
        al_draw_text(font, textColor, b1X + BUTTON_WIDTH/ 2, b1Y + 10, ALLEGRO_ALIGN_CENTER, "Accepter");

        int b2X =  boxX + boxW - BUTTON_WIDTH - margin;
        int b2Y = boxY + boxH - BUTTON_HEIGHT - margin;
        al_draw_filled_rectangle(b2X, b2Y, b2X + BUTTON_WIDTH, b2Y + BUTTON_HEIGHT, al_map_rgb(200, 0, 0));
        al_draw_text(font, textColor, b2X + BUTTON_WIDTH / 2, b2Y + 10, ALLEGRO_ALIGN_CENTER, "Refuser");

        al_flip_display();

        // Attente d'événements
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            int mx = ev.mouse.x;
            int my = ev.mouse.y;

            // Si clique sur Accepter
            if (mx >= b1X && mx <= b1X + BUTTON_WIDTH &&
                my >= b1Y && my <= b1Y + BUTTON_HEIGHT) {
                return TRADE_ACCEPT;
            }

            // Si clique sur Refuser
            if (mx >= b2X && mx <= b2X + BUTTON_WIDTH &&
                my >= b2Y && my <= b2Y + BUTTON_HEIGHT) {
                return TRADE_REFUSE;
            }
        }

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            return TRADE_REFUSE;
        }
    }
    return TRADE_REFUSE;
}
