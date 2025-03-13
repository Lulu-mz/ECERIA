#include <stdio.h>
#include "Joueur.h"
#include "Jeu.h"
#include "Menu.h"
#include "GestionDonnees.h"
#include "Animation.h"
#include "Carte.h"

int main(void)
{
    // animation();
     Joueur joueur;
     initJoueur(&joueur);
     printf("You have %d items in your bag.\n", compterItems(&joueur));
     if (!joueurEstInscrit(joueur.nom)) {
         enregistrerJoueurDansCSV(&joueur);
     } else {
         printf("Le joueur %s est deja inscrit, pas d'ajout.\n", joueur.nom);
     }
     menu(&joueur);
return 0;
}

