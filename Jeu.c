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

    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(1);
}

void registerEventSource(ALLEGRO_DISPLAY *display, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue) {
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());
}


void deplacerJoueur(Jeu* jeu) {
    Joueur* joueur = jeu->joueur;
    Carte* carte = getCurrentCarte(jeu);
    int next_x; //position en x à la prochaine case
    int next_y; //position en x à la prochaine case
    switch (joueur->direction) {
        case DROITE:
            next_x = (int) (joueur->x2 + joueur->speed) / TILE_SIZE;
            next_y = (int) joueur->y2 / TILE_SIZE;
            joueur->regard = RIGHT;
            if (joueur->x2 < WIDTH && next_x<WIDTH/TILE_SIZE&& carte->map[next_y][next_x].marchable == true && carte->map[(int) joueur->y1 / TILE_SIZE][
                    next_x].marchable == true) {
                joueur->x1 += joueur->speed;
                joueur->x2 += joueur->speed;
            }
            if(next_x >= WIDTH/TILE_SIZE) { //si on arrive au bord à droite d'une carte
                if(jeu->joueur->pos_j + 1 < jeu->mapSize) { //si on arrive au bord du jeu
                    if(jeu->sections[jeu->joueur->pos_i][jeu->joueur->pos_j+1] == NULL) {
                        jeu->sections[jeu->joueur->pos_i][jeu->joueur->pos_j+1] = creerCarte(WIDTH/TILE_SIZE, HEIGHT /TILE_SIZE);
                    }
                    jeu->joueur->pos_j++;
                    joueur->x1 = joueur->l;
                    joueur->x2 = joueur->x1 + joueur->l; //hitbox
                }
            }
            break;

        case GAUCHE:
            next_x = (int) (joueur->x1 - joueur->speed) / TILE_SIZE;
            next_y = (int) joueur->y1 / TILE_SIZE;
            joueur->regard = LEFT;
            if (joueur->x1 > 0.0f && carte->map[next_y][next_x].marchable == true && carte->map[(int) joueur->y2 / TILE_SIZE][
                    next_x].marchable == true) {
                joueur->x1 -= joueur->speed;
                joueur->x2 -= joueur->speed;
            }
            if(next_x<= 0) { //si on arrive au bord à gauche d'une carte
                if(jeu->joueur->pos_j - 1 >=0) { //si on arrive au bord du jeu
                    if(jeu->sections[jeu->joueur->pos_i][jeu->joueur->pos_j-1] == NULL) {
                        jeu->sections[jeu->joueur->pos_i][jeu->joueur->pos_j-1] = creerCarte(WIDTH/TILE_SIZE, HEIGHT /TILE_SIZE);
                    }
                    jeu->joueur->pos_j--;
                    joueur->x2 = WIDTH - joueur->l;
                    joueur->x1 = joueur->x2 - joueur->l; //hitbox
                }
            }
            break;

        case HAUT:
            next_x = (int) joueur->x1 / TILE_SIZE;
            next_y = (int) (joueur->y1 - joueur->speed) / TILE_SIZE;
            joueur->regard = UP;
            if (joueur->y1 > 0.0f && carte->map[next_y][next_x].marchable == true && carte->map[next_y][
                    (int) joueur->x2 / TILE_SIZE].marchable == true) {
                joueur->y1 -= joueur->speed;
                joueur->y2 -= joueur->speed;
            }
            if(next_y<= 0) { //si on arrive au bord en haut d'une carte
                if(jeu->joueur->pos_i - 1 >= 0) { //si on arrive au bord du jeu
                    if(jeu->sections[jeu->joueur->pos_i - 1][jeu->joueur->pos_j] == NULL) {
                        jeu->sections[jeu->joueur->pos_i - 1][jeu->joueur->pos_j] = creerCarte(WIDTH/TILE_SIZE, HEIGHT /TILE_SIZE);
                    }
                    jeu->joueur->pos_i--;
                    joueur->y2 = HEIGHT - joueur->h;
                    joueur->y1 = joueur->y2 - joueur->h; //hitbox
                }
            }
            break;

        case BAS:
            next_x = (int) joueur->x2 / TILE_SIZE;
            next_y = (int) (joueur->y2 + joueur->speed) / TILE_SIZE;
            joueur->regard = DOWN;
            if (joueur->y2< HEIGHT && next_y<HEIGHT/TILE_SIZE && carte->map[next_y][next_x].marchable == true && carte->map[next_y][
                    (int) joueur->x1 / TILE_SIZE].marchable == true) {
                joueur->y1 += joueur->speed;
                joueur->y2 += joueur->speed;
            }
            if(next_y >= HEIGHT/TILE_SIZE) { //si on arrive au bord en bas d'une carte
                if(jeu->joueur->inHouse != -1) { //si dans une maison
                    deplacerJoueurDevantMaison(jeu->sections[jeu->joueur->pos_i][jeu->joueur->pos_j], jeu->joueur, jeu->joueur->inHouse);
                    jeu->joueur->inHouse = -1; //on sort de la maison
                }
                else {
                    if(jeu->joueur->pos_i + 1 < jeu->mapSize) { //si on arrive au bord du jeu
                        if(jeu->sections[jeu->joueur->pos_i + 1][jeu->joueur->pos_j] == NULL) {//si pas dans une maison, au bord de la carte, on crée une new carte
                            jeu->sections[jeu->joueur->pos_i + 1][jeu->joueur->pos_j] = creerCarte(WIDTH/TILE_SIZE, HEIGHT /TILE_SIZE);
                        }
                        jeu->joueur->pos_i++;
                        joueur->y1 = joueur->h;
                        joueur->y2 = joueur->y1 + joueur->h; //hitbox
                    }
                }
            }
            break;

        default: //direction par défaut = statique
            break;
    }
}


int animation() {
    installation();

    ALLEGRO_DISPLAY *window = al_create_display(WIDTH, HEIGHT);
    if(!window) {
        fprintf(stderr, "Erreur : Impossible de creer la fenetre.\n");
        return -1;
    }

    ALLEGRO_SAMPLE* sample = NULL;

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    if (!queue) {
        fprintf(stderr, "Erreur : Impossible de creer la file d'evenements.\n");
        cleanUp(NULL, NULL, window, NULL, sample);
        return -1;
    }

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);
    if (!timer) {
        fprintf(stderr, "Erreur : Impossible de creer le timer.\n");
        cleanUp(NULL, queue, window, NULL, sample);
        return -1;
    }

    registerEventSource(window, timer, queue);
    al_start_timer(timer);

    ALLEGRO_KEYBOARD_STATE keyboard_state;

    int currentFrame = 0;
    int frameTime = 0;
    int totalFrame = 0;

    Jeu* jeu = menu(queue);  // Chargement du menu + carte choisie
    if (!jeu) {
        fprintf(stderr, "Erreur : Chargement du jeu échoué.\n");
        cleanUp(timer, queue, window, NULL, sample);
        return -1;
    }

    if (!al_install_audio() || !al_init_acodec_addon()) {
        fprintf(stderr, "Erreur d'initialisation audio.\n");
        cleanUp(timer, queue, window, jeu, sample);
        return -1;
    }

    srand(time(NULL));

    bool afficherDialogue = false;
    int dialogueTimer = 0;
    int dialogueX = 0, dialogueY = 0;

    char* texteDialogue = NULL;

    bool running = true;

    while (running) {
        Carte* carte = getCurrentCarte(jeu);
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        al_get_keyboard_state(&keyboard_state);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }

        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_E) {
            int px, py;
            action(jeu->joueur, carte, &px, &py);
            if (px >= 0 && py >= 0 && carte->map[py][px].pnj != NULL) {
                afficherDialogue = true;
                dialogueX = px;
                dialogueY = py;
                texteDialogue = randomText();
                dialogueTimer = 180; // durée de la bulle de dialogue en sec

                // On regarde si un message de trade doit être affiché :
                const char* tradeMsg = trade(jeu->joueur, carte->map[py][px].pnj, queue, px, py, carte);
                if(tradeMsg != NULL) {
                    texteDialogue = tradeMsg;
                    dialogueTimer = 180; // durée de la bulle de dialogue en sec
                }
            }
        }

        if (event.type == ALLEGRO_EVENT_TIMER) {
            afficherCarte(carte, totalFrame);
            afficherJoueur(jeu->joueur, currentFrame);
            if (afficherDialogue && dialogueTimer > 0 && texteDialogue != NULL) {
                afficherBulleDialogue(dialogueX, dialogueY, texteDialogue);
                dialogueTimer--;
                if (dialogueTimer <= 0) {
                    afficherDialogue = false;
                    texteDialogue = NULL;
                }
            }
            al_flip_display();
        }

        // Mouvement joueur
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_D)) {
            jeu->joueur->direction = DROITE;
        } else if (al_key_down(&keyboard_state, ALLEGRO_KEY_Q)) {
            jeu->joueur->direction = GAUCHE;
        } else if (al_key_down(&keyboard_state, ALLEGRO_KEY_Z)) {
            jeu->joueur->direction = HAUT;
        } else if (al_key_down(&keyboard_state, ALLEGRO_KEY_S)) {
            jeu->joueur->direction = BAS;
        } else {
            jeu->joueur->direction = STATIQUE;
        }

        deplacerJoueur(jeu);
        playSound(jeu);

        frameTime += 16;
        if (frameTime >= 600) {
            currentFrame = (currentFrame + 1) % 2;
            totalFrame = (totalFrame + 1) % 2000000000;
            frameTime = 0;
        }
    }

    saveJeu(jeu);
    cleanUp(timer, queue, window, jeu, sample);

    return 0;
}

Carte* getCurrentCarte(Jeu* jeu) {
    if(jeu->joueur->inHouse == -1) {
        return (jeu->sections[jeu->joueur->pos_i][jeu->joueur->pos_j]);
    }
    return (jeu->sections[jeu->joueur->pos_i][jeu->joueur->pos_j]->carte_maison[jeu->joueur->inHouse]);
}

void afficherButton(ALLEGRO_BITMAP *buttonImg, ButtonState *btn) {
    float drawX = btn->x - ((btn->scale - 1.0f) * btn->width / 2.0f);
    float drawY = btn->y - ((btn->scale - 1.0f) * btn->height / 2.0f);
    float drawWidth = btn->width * btn->scale;
    float drawHeight = btn->height * btn->scale;

    ALLEGRO_COLOR tint = al_map_rgba_f(btn->alpha, btn->alpha, btn->alpha, btn->alpha);

    al_draw_tinted_scaled_bitmap(
        buttonImg,
        tint,
        0, 0, 697, 337, // Taille réelle de ton image source
        drawX,
        drawY,
        drawWidth,
        drawHeight,
        0
    );
}


void afficherMenu(ALLEGRO_BITMAP* background, ALLEGRO_BITMAP* buttonImg,
                  ButtonState *btnNewGame, ButtonState *btnLoadGame) {
    al_draw_bitmap(background, 0, 0, 0);

    afficherButton(buttonImg, btnNewGame);
    afficherButton(buttonImg, btnLoadGame);

    ALLEGRO_FONT *font = al_load_ttf_font("../Assets/Arial.ttf", 20, 0);
    if (font) {
        al_draw_text(font, al_map_rgb(0, 0, 0), WIDTH / 2, btnNewGame->y + 25, ALLEGRO_ALIGN_CENTRE, "Nouvelle partie");
        al_draw_text(font, al_map_rgb(0, 0, 0), WIDTH / 2, btnLoadGame->y + 25, ALLEGRO_ALIGN_CENTRE, "Charger une partie");
        al_destroy_font(font);
    } else {
        fprintf(stderr, "Erreur lors de la création de la police.\n");
    }
}


Jeu* menu(ALLEGRO_EVENT_QUEUE *queue) {
    ALLEGRO_BITMAP *background = al_load_bitmap("../Assets/background_2.jpg");
    ALLEGRO_BITMAP *buttonImg = al_load_bitmap("../Assets/button.png");

    if (!background || !buttonImg) {
        fprintf(stderr, "Erreur de chargement d'image.\n");
        return NULL;
    }

    Jeu* jeu = NULL;
    bool in_menu = true;

    ButtonState btnNewGame = { (WIDTH - 300) / 2.0f, 300, 300, 80, false, false, 1.0f, 1.0f };
    ButtonState btnLoadGame = { btnNewGame.x, btnNewGame.y + 110, 300, 80, false, false, 1.0f, 1.0f };

    while (in_menu) {
        ALLEGRO_MOUSE_STATE mouse;
        al_get_mouse_state(&mouse);
        int mx = mouse.x;
        int my = mouse.y;

        // Gestion Hover + Click bouton Nouvelle partie
        btnNewGame.hovered = (mx >= btnNewGame.x && mx <= btnNewGame.x + btnNewGame.width &&
                              my >= btnNewGame.y && my <= btnNewGame.y + btnNewGame.height);
        btnNewGame.clicked = btnNewGame.hovered && (mouse.buttons & 1);
        btnNewGame.scale = btnNewGame.hovered ? 1.1f : 1.0f;
        btnNewGame.alpha = btnNewGame.clicked ? 0.5f : 1.0f;

        // Gestion Hover + Click bouton Charger
        btnLoadGame.hovered = (mx >= btnLoadGame.x && mx <= btnLoadGame.x + btnLoadGame.width &&
                               my >= btnLoadGame.y && my <= btnLoadGame.y + btnLoadGame.height);
        btnLoadGame.clicked = btnLoadGame.hovered && (mouse.buttons & 1);
        btnLoadGame.scale = btnLoadGame.hovered ? 1.1f : 1.0f;
        btnLoadGame.alpha = btnLoadGame.clicked ? 0.5f : 1.0f;

        afficherMenu(background, buttonImg, &btnNewGame, &btnLoadGame);
        al_flip_display();

        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jeu = NULL;
            in_menu = false;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {
            if (btnNewGame.hovered) {
                jeu = nouvellePartie();
                in_menu = false;
            } else if (btnLoadGame.hovered) {
                jeu = chargerPartie();
                in_menu = false;
            }
        }
    }

    al_destroy_bitmap(background);
    al_destroy_bitmap(buttonImg);
    return jeu;
}


Jeu* nouvellePartie() {
    Jeu* jeu = malloc(1*sizeof(Jeu));
    jeu->mapSize = MAP_SIZE;
    jeu->sections = malloc(jeu->mapSize*sizeof(Carte**));
    for (int i = 0; i < jeu->mapSize; i++) {
        jeu->sections[i] = malloc(jeu->mapSize*sizeof(Carte*));
    }
    char buffer[50];
    for(int i = 0; i < jeu->mapSize; i++) {
        for(int j = 0; j < jeu->mapSize; j++) {
            for(int k = 0; k < 5; k++) {
                sprintf(buffer, "../Save/house_%d_%d_%d.txt",i,j,k);
                remove(buffer);
            }
            jeu->sections[i][j] = NULL;
            sprintf(buffer, "../Save/map_%d_%d.txt",i,j);
            remove(buffer);
            sprintf(buffer, "../Save/biome_%d_%d.txt",i,j);
            remove(buffer);
        }
    }
    jeu->joueur = createJoueur();
    jeu->sections[jeu->joueur->pos_i][jeu->joueur->pos_j] = creerCarte(WIDTH/TILE_SIZE, HEIGHT /TILE_SIZE);
    saveJeu(jeu);
    return jeu;
}


Jeu* chargerPartie() {
    Jeu* jeu = malloc(1*sizeof(Jeu));
    jeu->mapSize = MAP_SIZE;
    jeu->sections = malloc(jeu->mapSize*sizeof(Carte**));
    for (int i = 0; i < jeu->mapSize; i++) {
        jeu->sections[i] = malloc(jeu->mapSize*sizeof(Carte*));
    }
    for(int i = 0; i < jeu->mapSize; i++) {
        for(int j = 0; j < jeu->mapSize; j++) {
            jeu->sections[i][j] = chargerCarte(WIDTH / TILE_SIZE, HEIGHT / TILE_SIZE, i, j);
            if(jeu->sections[i][j] != NULL) {
                for(int k = 0; k < jeu->sections[i][j]->nbMaison; k++) {
                    jeu->sections[i][j]->carte_maison[k] = chargerInterieurMaison(HEIGHT / TILE_SIZE, WIDTH / TILE_SIZE, i, j, k);
                    //on charge les k intérieur des maisons
                }
            }
        }
    }
    jeu->joueur = chargerJoueur();
    return jeu;
}


void saveJeu(Jeu* jeu) {
    saveJoueur(jeu->joueur);
    for(int i = 0; i < jeu->mapSize; i++) {
        for(int j = 0; j < jeu->mapSize; j++) {
            if(jeu->sections[i][j] != NULL) {
                saveCarte(jeu->sections[i][j], i, j);
                for(int k = 0; k < jeu->sections[i][j]->nbMaison; k++) {
                    if(jeu->sections[i][j]->carte_maison[k] != NULL) {
                        saveInterieurMaison(jeu->sections[i][j]->carte_maison[k], i, j, k);
                    }
                }
            }
        }
    }
}


void destroyJeu(Jeu* jeu) {
    destroyJoueur(jeu->joueur);
    for(int i = 0; i < jeu->mapSize; i++) {
        for(int j = 0; j < jeu->mapSize; j++) {
            if(jeu->sections[i][j]!= NULL) {
                destroyCarte(jeu->sections[i][j]);
            }
        }
    }
    for (int i = 0; i < jeu->mapSize; i++) {
        free(jeu->sections[i]);
    }
    free(jeu->sections);
    free(jeu);
}


int playSound(Jeu* jeu) {
    if (jeu == NULL || jeu->joueur == NULL || jeu->joueur->direction == STATIQUE) {
        return 1;
    }

    ALLEGRO_SAMPLE *grass_walk = al_load_sample("../Sounds/walking_on_grass_2.wav");
    ALLEGRO_SAMPLE *floor_walk = al_load_sample("../Sounds/walking_on_floor.wav");
    if (!grass_walk || !floor_walk) {
        fprintf(stderr, "Impossible de charger le fichier wav\n");
        return 1;
    }

    if(jeu->joueur->inHouse != -1) {
        al_play_sample(floor_walk, 1.0f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else {
        al_play_sample(grass_walk, 1.0f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    return 0;
}

char* randomText() {
    int index = rand()%4;

    char* text[4] = {
        "Hey !",
        "Comment ça va ?",
        "Quelle belle journée !",
        "Souhaites tu échanger quelque chose de ton inventaire ?"
    };

    return text[index];
}

void afficherBulleDialogue(int tile_x, int tile_y, const char* textToPrint) {
    float padding = 10;
    float rayon = 8;

    // Créer la police
    ALLEGRO_FONT* font = al_create_builtin_font();
    if (!font) return;

    // Calcul largeur et hauteur texte
    float largeur_texte = al_get_text_width(font, textToPrint);
    float hauteur_texte = al_get_font_line_height(font);

    // Taille de la bulle
    float largeur = largeur_texte + 2 * padding;
    float hauteur = hauteur_texte + 2 * padding;

    // Position écran en pixels (coin supérieur gauche de la bulle)
    float x = tile_x * TILE_SIZE;
    float y = tile_y * TILE_SIZE - hauteur - 10;

    // Couleurs
    ALLEGRO_COLOR fond = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR bordure = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR texte = al_map_rgb(0, 0, 0);

    // Affichage bulle
    al_draw_filled_rounded_rectangle(x, y, x + largeur, y + hauteur, rayon, rayon, fond);
    al_draw_rounded_rectangle(x, y, x + largeur, y + hauteur, rayon, rayon, bordure, 2);

    // Texte centré
    al_draw_text(font, texte, x + largeur / 2, y + hauteur / 2 - hauteur_texte / 2, ALLEGRO_ALIGN_CENTER, textToPrint);

    al_destroy_font(font);
}


void cleanUp(ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_DISPLAY *window, Jeu *jeu, ALLEGRO_SAMPLE *sample) {
    if (jeu) destroyJeu(jeu);
    if (timer) al_destroy_timer(timer);
    if (queue) al_destroy_event_queue(queue);
    if (window) al_destroy_display(window);
    if (sample) al_destroy_sample(sample);

    // Important : désinstaller l'audio **après** avoir détruit les samples
    al_uninstall_audio();
}

