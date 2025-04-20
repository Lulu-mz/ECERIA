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
    al_init_font_addon();
    al_init_ttf_addon();
}

void registerEventSource(ALLEGRO_DISPLAY *display, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue) {
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());
}


int animation() {
    installation();

    ALLEGRO_DISPLAY *window = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);

    registerEventSource(window, timer, queue);

    al_start_timer(timer);

    Joueur *joueur = NULL;
    ALLEGRO_KEYBOARD_STATE keyboard_state;

    int currentFrame = 0;
    int frameTime = 0;

    Carte* carte = NULL;
    menu(queue, &carte, &joueur); // Chargement du menu + carte choisie
    if (!carte || !joueur) {
        al_destroy_timer(timer);
        al_destroy_event_queue(queue);
        al_destroy_display(window);
        return 0;
    }

    bool running = true;

    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        al_get_keyboard_state(&keyboard_state);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }

        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_E) {
            action(joueur, carte);
        }

        if (event.type == ALLEGRO_EVENT_TIMER) {
            afficherCarte(carte);
            afficherJoueur(joueur, currentFrame);
            al_flip_display();
        }

        // Mouvement joueur
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_D)) {
            joueur->direction = DROITE;
        } else if (al_key_down(&keyboard_state, ALLEGRO_KEY_Q)) {
            joueur->direction = GAUCHE;
        } else if (al_key_down(&keyboard_state, ALLEGRO_KEY_Z)) {
            joueur->direction = HAUT;
        } else if (al_key_down(&keyboard_state, ALLEGRO_KEY_S)) {
            joueur->direction = BAS;
        } else {
            joueur->direction = STATIQUE;
        }

        deplacerJoueur(joueur, carte);

        frameTime += 16;
        if (frameTime >= 600) {
            currentFrame = (currentFrame + 1) % 2;
            frameTime = 0;
        }
    }

    saveCarte(carte);
    saveJoueur(joueur);
    destroyJoueur(joueur);
    destroyCarte(carte);

    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(window);

    return 0;
}


void afficherMenu(ALLEGRO_BITMAP* menu_background, ALLEGRO_BITMAP* button) {
    al_draw_bitmap(menu_background, 0, 0, 0);

    // Position des boutons
    float button_width = 300;
    float button_height = 80;
    float button_x = (WIDTH - button_width) / 2.0f;
    float first_button_y = 300;
    float second_button_y = first_button_y + button_height + 30;

    // Dessiner les deux boutons redimensionnés
    al_draw_scaled_bitmap(button, 0, 0, 697, 337, button_x, first_button_y, button_width, button_height, 0);
    al_draw_scaled_bitmap(button, 0, 0, 697, 337, button_x, second_button_y, button_width, button_height, 0);

    // Affichage du texte
    ALLEGRO_FONT *font = al_load_ttf_font("../Assets/Arial.ttf", 20, 0);
    if (font) {
        al_draw_text(font, al_map_rgb(0, 0, 0), WIDTH / 2, first_button_y + 20, ALLEGRO_ALIGN_CENTRE, "Nouvelle partie");
        al_draw_text(font, al_map_rgb(0, 0, 0), WIDTH / 2, second_button_y + 20, ALLEGRO_ALIGN_CENTRE, "Charger une partie");
        al_destroy_font(font);
    } else {
        fprintf(stderr, "Erreur lors de la création de la police.\n");
    }
}


void menu(ALLEGRO_EVENT_QUEUE *queue, Carte** carte, Joueur** joueur) {
    ALLEGRO_BITMAP *background = al_load_bitmap("../Assets/background_2.jpg");
    ALLEGRO_BITMAP *button = al_load_bitmap("../Assets/button.png");

    if (!background || !button) {
        fprintf(stderr, "Erreur de chargement d'image.\n");
        return;
    }

    bool in_menu = true;

    while (in_menu) {
        afficherMenu(background, button);
        al_flip_display();

        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            carte = NULL;
            joueur = NULL;
            in_menu = false;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {
            int x = event.mouse.x;
            int y = event.mouse.y;

            float button_width = 300;
            float button_height = 80;
            float button_x = (WIDTH - button_width) / 2.0f;
            float first_button_y = 300;
            float second_button_y = first_button_y + button_height + 30;

            if (x >= button_x && x <= button_x + button_width &&
                y >= first_button_y && y <= first_button_y + button_height) {
                *carte = creerCarte(WIDTH/TILE_SIZE, HEIGHT /TILE_SIZE);
                saveCarte(*carte);
                *joueur = createJoueur();
                saveJoueur(*joueur);
                in_menu = false;
                } else if (x >= button_x && x <= button_x + button_width &&
                           y >= second_button_y && y <= second_button_y + button_height) {
                    *carte = chargerCarte(WIDTH / TILE_SIZE, HEIGHT / TILE_SIZE);
                    *joueur = chargerJoueur();
                    in_menu = false;
                           }
        }
    }

    al_destroy_bitmap(background);
    al_destroy_bitmap(button);
}