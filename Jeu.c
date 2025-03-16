#include "Jeu.h"
#include "Carte.h"
#include "Joueur.h"

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
            afficherJoueur(joueur);
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
