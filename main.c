/* 
 * File:   main.c
 * Author: Cedric
 *
 * Created on 6 mai 2011, 02:18
 */

#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"

/*
 * 
 */
int main(int argc, char** argv) {
    // Test du programme pour un plateau donné
    // On peut essayer par exemple dureeLimite = 5 et dureeLimite = 15 : on aura 2 resultats différents

    Etat *plateau;
    Etat etat = Etat_init();
    Coup coupAJouer;
    Pos pos;
    float dureeLimite = 15.; // L'algorithme de décision voit à dureeLimite secs d'avance
    plateau = &etat;

    // Position du robot
    plateau->robotAmi.x = 2.;
    plateau->robotAmi.y = 1.;

    // Positionnement de 5 pions
    pos.x = 0.;
    pos.y = 2.;
    Etat_ajouterPion(plateau, pos);

    pos.x = 2.;
    pos.y = 0.;
    Etat_ajouterPion(plateau, pos);

    pos.x = 3.;
    pos.y = 3.;
    Etat_ajouterPion(plateau, pos);

    pos.x = 1.;
    pos.y = 4.;
    Etat_ajouterPion(plateau, pos);

    pos.x = 5.;
    pos.y = 1.;
    Etat_ajouterPion(plateau, pos);

    // Positionnement d'une figure
    pos.x = 4;
    pos.y = 2;
    Etat_ajouterFigure(plateau, pos);

    // Prise de décision
    coupAJouer = deciderCoup(plateau, dureeLimite);

    printf("Coup a jouer :\n");
    if (coupAJouer.tCoup == PION) {
        Pos pion = coupAJouer.uCoup.coupPion.pion;
        Pos dest = coupAJouer.uCoup.coupPion.dest;
        printf("Pousser le pion : [%.1f, %.1f]\n", pion.x, pion.y);
        printf("en : [%.1f, %.1f]\n", dest.x, dest.y);
    } else { // TOUR
        Tour tour = coupAJouer.uCoup.coupTour.tour;
        Pos figure = tour.figure;
        Pos pion1 = tour.pion1;
        Pos pion2 = tour.pion2;
        Pos dest = coupAJouer.uCoup.coupTour.dest;
        printf("Assembler la tour avec :\n");
        printf("La figure : [%.1f, %.1f]\n", figure.x, figure.y);
        printf("Le premier pion : [%.1f, %.1f]\n", pion1.x, pion2.y);
        printf("Le second pion : [%.1f, %.1f]\n", pion2.x, pion2.y);
        printf("Et pousser la tour en : [%.1f, %.1f]\n", dest.x, dest.y);
    }

    getchar();

    return (EXIT_SUCCESS);
}

