#include "Animation.h"
#include "Carte.h"

//A METTRE DANS Joueur.h
typedef enum direction {
    DROITE, GAUCHE, HAUT, BAS, STATIQUE
}Direction;

//A METTRE DANS Joueur.h
typedef struct j {
    float speed;
    float x1, y1;
    float x2, y2;
    Direction direction;
    ALLEGRO_BITMAP* image;
}Joueur;



//A METTRE DANS Joueur.c
Joueur* createJoueur() {
    Joueur* joueur = calloc(1,sizeof(Joueur));
    joueur->x1 = (float) WIDTH/2; //faire démarer au milieu
    joueur->y1 = (float) HEIGHT/2; //faire démarer au milieu
    joueur->x2 = joueur->x1 + 32.0f; //taille du personnage
    joueur->y2 = joueur->y1 + 32.0f; //taille du personnage
    joueur->speed = 5.0f; //vitesse de déplacement
    joueur->direction = STATIQUE; //direction par défaut = statique

    ALLEGRO_BITMAP *sprite_sheet = al_load_bitmap("../Spritesheet/character.png");
    if(!sprite_sheet) {
        printf("Erreur de chargement de l'image\n");
        exit(1);
    }
    joueur->image = al_create_sub_bitmap(sprite_sheet, 1, 1, 16, 16);//image découpé depuis le fichier pour avoir un personnage
    return joueur;
}

//A METTRE DANS Joueur.c
void destroyJoueur(Joueur* joueur) {
    free(joueur);
    joueur = NULL;
}

//A METTRE DANS Joueur.c
void afficherJoueur(Joueur* joueur, ALLEGRO_DISPLAY* window) {
    al_draw_scaled_bitmap(joueur->image, 0, 0, 16, 16, joueur->x1, joueur->y1, 16*2, 16*2, 0); //agrandir le personnnage de 16p -> 32p
}

//A METTRE DANS Joueur.c
void deplacerJoueur(Joueur* joueur) {
    switch (joueur->direction) {
        case DROITE :
            if(joueur->x2 < WIDTH) {
                joueur->x1 += joueur->speed;
                joueur->x2 += joueur->speed;
            }
        break;

        case GAUCHE :
            if (joueur->x1 > 0.0f) {
                joueur->x1 -= joueur->speed;
                joueur->x2 -= joueur->speed;
            }
        break;

        case HAUT :
            if(joueur->y1 > 0.0f) {
                joueur->y1 -= joueur->speed;
                joueur->y2 -= joueur->speed;
            }
        break;

        case BAS :
            if(joueur->y2 < HEIGHT) {
                joueur->y1 += joueur->speed;
                joueur->y2 += joueur->speed;
            }
        break;

        default : //direction par défaut = statique
            break;
    }
}

void installation() {
    if (!al_init()) {
        printf("Une erreur est survenue lors de l'installation\n");
        exit(1);
    }
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_image_addon();
}

void registerEventSource(ALLEGRO_DISPLAY *display, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue) {
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());
}

int animation()
{
    installation();

    ALLEGRO_DISPLAY *window = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60); // 60 FPS

    registerEventSource(window, timer, queue);

    bool running = true;
    al_start_timer(timer);
    Joueur *joueur = createJoueur();
    Carte* carte = chargerCarte(WIDTH/16, HEIGHT/16);

    ALLEGRO_KEYBOARD_STATE keyboard_state;

    while (running)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        al_get_keyboard_state(&keyboard_state);
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            running = false;
        }
        if(event.type == ALLEGRO_EVENT_TIMER){
            // al_clear_to_color(al_map_rgb(0,0,0));
            afficherCarte(carte);
            afficherJoueur(joueur, window);
            al_flip_display();
        }
        if(al_key_down(&keyboard_state,ALLEGRO_KEY_RIGHT)) {
            joueur->direction = DROITE;
            deplacerJoueur(joueur);
        }
        if(al_key_down(&keyboard_state,ALLEGRO_KEY_LEFT)) {
            joueur->direction = GAUCHE;
            deplacerJoueur(joueur);
        }
        if(al_key_down(&keyboard_state,ALLEGRO_KEY_UP)) {
            joueur->direction = HAUT;
            deplacerJoueur(joueur);
        }
        if(al_key_down(&keyboard_state,ALLEGRO_KEY_DOWN)) {
            joueur->direction = BAS;
            deplacerJoueur(joueur);
        }
    }
    destroyJoueur(joueur);
    destroyCarte(carte);
    return 0;
}
