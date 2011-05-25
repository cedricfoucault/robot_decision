/*
 * File:   jeu.h
 * Author: Cedric
 *
 * Created on 6 mai 2011, 00:00
 */

#ifndef JEU_H
#define	JEU_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

    // /!\ CONSTANTES A DEFINIR EN FONCTIONS DES CAPACITES DU ROBOT
#define VITESSE_MOY (0.5)   /* Vitesse moyenne de déplacement du robot
                             * (VITESSE_MOY = 1 <=> 1 longueur de case / seconde) */
#define TEMPS_SUP (5) /* Temps supplémentaire moyen (en secondes)
                       * qu'il faut au robot pour assembler une tour,
                       * en plus du temps de déplacement :
                       * pour pincer et soulever la tour, la reposer, se tourner... */

    // Pos : Type pour représenter une position sur le damier

    typedef struct {
        float x; // longueur d'un intervalle séparant chaque case = 1
        float y;
    } Pos;
    // Getters
    float Pos_getX(Pos *pos);
    float Pos_getY(Pos *pos);
    // Setters
    void Pos_setX(Pos *pos, float x);
    void Pos_setY(Pos *pos, float y);

    bool Pos_egal(Pos pos1, Pos pos2);
    float Pos_distance(Pos pos1, Pos pos2);

    // Etat : Type pour représenter un état du plateau à un instant donné du jeu

    typedef struct {
        unsigned char nbPions; // nb de pions dont on est "sûr" de la position
        // (1 pions disparaît toutes les X secs si on
        //  a pas pu vérifier son existence)
        unsigned char nbFigures; // idem
        unsigned short points;
        Pos robotAmi; // position estimée de notre robot
        //Pos robotEnnemi; // position estimée du robot adverse : paramètre optionnel ?
        Pos pions[15]; // position des nbPions pions
        Pos figures[4]; // idem
        float temps; // temps écoulé
    } Etat;
    // Getters
    unsigned char Etat_getNbPions(Etat *plateau);
    unsigned char Etat_getNbFigures(Etat *plateau);
    Pos Etat_getRobotAmi(Etat *plateau);
    Pos *Etat_getPions(Etat *plateau);
    Pos *Etat_getFigures(Etat *plateau);
    float Etat_getTemps(Etat *plateau);
    // Setters
    void Etat_setNbPions(Etat *plateau, unsigned char nbPions);
    void Etat_setNbFigures(Etat *plateau, unsigned char nbFigures);
    void Etat_setRobotAmi(Etat *plateau, Pos robotAmi);
    void Etat_setPions(Etat *plateau, Pos pions[]);
    void Etat_setFigures(Etat *plateau, Pos figures[]);
    void Etat_setTemps(Etat *plateau, float temps);
    // Copies
    Etat *Etat_copyEtat(Etat *plateau);
    //Pos *Etat_copyPions(Etat *plateau);
    //Pos *Etat_copyFigures(Etat *plateau);
    Etat Etat_init();
    void Etat_ajouterPion(Etat *plateau, Pos pion);
    void Etat_ajouterFigure(Etat *plateau, Pos figure);
    bool Etat_bougerPion(Etat *plateau, Pos pion, Pos dest);
    bool Etat_bougerFigure(Etat *plateau, Pos figure, Pos dest);
    bool Etat_detruirePion(Etat *plateau, Pos pion); /* Détruit un pion du damier
                                                         * retourne FALSE si aucun
                                                         * pion n'a été détruit */
    bool Etat_detruireFigure(Etat *plateau, Pos figure); /* idem */
    float Etat_heuristique(Etat *plateau); /* Détermine l'heuristique actuelle du plateau de jeu */


    // Coup : type pour représenter un coup à jouer
    // il s'agit soit d'un palet à déplacer sur une case (coupPion)
    // soit d'une tour à assembler puis à déplacer (coupTour)

    typedef struct {
        Pos pion; // position du pion à déplacer
        Pos dest; // destination (case où poser/pousser le pion)
        Pos robot; // position du robot après avoir déplacé la pion
    } CoupPion;
    // Getters
    Pos CoupPion_getPion(CoupPion *coup);
    Pos CoupPion_getDest(CoupPion *coup);
    Pos CoupPion_getRobot(CoupPion *coup);
    // Setters
    void CoupPion_setPion(CoupPion *coup, Pos pion);
    void CoupPion_setDest(CoupPion *coup, Pos dest);
    void CoupPion_setRobot(CoupPion *coup, Pos robot);

    typedef struct Tour_ {
        Pos figure;
        Pos pion1;
        Pos pion2;
    } Tour;
    // Getters
    Pos Tour_getFigure(Tour *tour);
    Pos Tour_getPion1(Tour *tour);
    Pos Tour_getPion2(Tour *tour);
    // Setters
    void Tour_setFigure(Tour *tour, Pos figure);
    void Tour_setPion1(Tour *tour, Pos pion1);
    void Tour_setPion2(Tour *tour, Pos pion2);

    typedef struct {
        Tour tour;
        Pos dest;
        Pos robot;
    } CoupTour;
    // Getters
    Tour CoupTour_getTour(CoupTour *coup);
    Pos CoupTour_getDest(CoupTour *coup);
    Pos CoupTour_getRobot(CoupTour *coup);
    // Setters
    void CoupTour_setTour(CoupTour *coup, Tour tour);
    void CoupTour_setDest(CoupPion *coup, Pos dest);
    void CoupTour_setRobot(CoupTour *coup, Pos robot);

    Pos calculerRobot(Pos pion, Pos dest); /* calcule la nouvelle position
                                            * du robot en fonction de celle du
                                            * pion choisi et de sa destination */

    typedef union {
        CoupPion coupPion;
        CoupTour coupTour;
    } UCoup;

    typedef enum {
        PION, TOUR
    } TCoup;

    typedef struct {
        UCoup uCoup; // le coup peut être un coupPion ou un coupTour
        TCoup tCoup; // indique le type du coup actuel (pion ou tour)
    } Coup;
    // Getters
    UCoup Coup_getUCoup(Coup *coup);
    TCoup Coup_getTCoup(Coup *coup);
    // Setters
    void Coup_setUCoup(Coup *coup, UCoup uCoup);
    void Coup_setTCoup(Coup *coup, TCoup tCoup);


    // Fonctions pour interagir avec le plateau

    Coup deciderCoup(Etat *plateau, float dureeLimitee);
    Coup *coupsPossibles(Etat *plateau, int *nbCoups);
    bool jouerCoup(Etat *plateau, Coup *coup);
    bool jouerCoupPion(Etat *plateau, CoupPion *coup);
    bool jouerCoupTour(Etat *plateau, CoupTour *coup);
    bool coupValide(Etat *plateau, Coup *coup);
    float estimerTemps(Etat *plateau, Coup *coup); /* retourne une estimée
                                                    * du temps nécessaire
                                                    * pour jouer coup */
    float estimerTempsPion(Etat *plateau, CoupPion *coup);
    float estimerTempsTour(Etat *plateau, CoupTour *coup);
    Tour deciderTour(Etat *plateau); /* détermine la manière optimale
                                      * d'assembler une tour
                                      * pour un état donné du plateau */


#ifdef	__cplusplus
}
#endif

#endif	/* JEU_H */

