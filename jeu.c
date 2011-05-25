#include "jeu.h"

// ## Fonctions associées à la struct Pos

// Getters

float Pos_getX(Pos *pos) {
    return pos->x;
}

float Pos_getY(Pos *pos) {
    return pos->y;
}
// Setters

void Pos_setX(Pos *pos, float x) {
    pos->x = x;
}

void Pos_setY(Pos *pos, float y) {
    pos->y = y;
}

float Pos_distance(Pos pos1, Pos pos2) {
    float x1 = pos1.x, y1 = pos1.y, x2 = pos2.x, y2 = pos2.y;
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

bool Pos_egal(Pos pos1, Pos pos2) { // "Environ égal" (+ ou - 0.4 près, 0.5 étant la demi-largeur d'1 case)
    //if ((pos1.x - 0.4 < pos2.x && pos2.x < pos1.x + 0.4) && (pos1.y - 0.4 < pos2.y && pos2.y < pos1.y + 0.4)) {
    if (pos1.x == pos2.x && pos1.y == pos2.y) {
        return true;
    } else {
        return false;
    }
}

unsigned short calculerPointsPion(Pos *pion) {
    float x = pion->x, y = pion->y;
    // Cases bonus : 10+30 points
    if (x < 4 + 0.5 && x > 4 - 0.5) {
        if ((y < 3 + 0.5 && y > 3 - 0.5) || (y < 1 + 0.5 && y > 1 - 0.5)) {
            return 40;
        }
    }
    if (x < 2 + 0.5 && x > 2 - 0.5 && y < 5 + 0.5 && y > 5 - 0.5) {
        return 40;
    }
    // 10 points sinon
    return 10;
}

unsigned short calculerPointsTour(Pos *tour) {
    float x = tour->x, y = tour->y;
    // Cases bonus : 30 points bonus
    if (x < 4 + 0.5 && x > 4 - 0.5) {
        if ((y < 3 + 0.5 && y > 3 - 0.5) || (y < 1 + 0.5 && y > 1 - 0.5)) {
            return 30 + 75;
        }
    }
    if (x < 2 + 0.5 && x > 2 - 0.5 && y < 5 + 0.5 && y > 5 - 0.5) {
        return 30 + 75;
    }
    // 25*3 = 75 points sinon
    return 30;
}
// ## Fonctions associées à la struct Etat

// Getters

unsigned char Etat_getNbPions(Etat *plateau) {
    return plateau->nbPions;
}

unsigned char Etat_getNbFigures(Etat *plateau) {
    return plateau->nbFigures;
}

Pos Etat_getRobotAmi(Etat *plateau) {
    return plateau->robotAmi;
}

Pos *Etat_getPions(Etat *plateau) {
    return plateau->pions;
}

Pos *Etat_getFigures(Etat *plateau) {
    return plateau->figures;
}

float Etat_getTemps(Etat *plateau) {
    return plateau->temps;
}

// Setters

void Etat_setNbPions(Etat *plateau, unsigned char nbPions) {
    plateau->nbPions = nbPions;
}

void Etat_setNbFigures(Etat *plateau, unsigned char nbFigures) {
    plateau->nbFigures = nbFigures;
}

void Etat_setRobotAmi(Etat *plateau, Pos robotAmi) {
    plateau->robotAmi = robotAmi;
}

void Etat_setPions(Etat *plateau, Pos pions[]) {
    int i;
    for (i = 0; i < plateau->nbPions; i++) {
        plateau->pions[i] = pions[i];
    }
}

void Etat_setFigures(Etat *plateau, Pos figures[]) {
    int i;
    for (i = 0; i < plateau->nbFigures; i++) {
        plateau->figures[i] = figures[i];
    }
}

void Etat_setTemps(Etat *plateau, float temps) {
    plateau->temps = temps;
}

// Copies

Etat *Etat_copyEtat(Etat *plateau) {
    Etat *plateauCopy;
    plateauCopy = (Etat*) malloc(sizeof(Etat));
    int nbPions = plateau->nbPions, nbFigures = plateau->nbFigures, i;
    plateauCopy->nbPions = nbPions;
    plateauCopy->nbFigures = nbFigures;
    for (i = 0; i < nbPions; i++) {
        plateauCopy->pions[i] = plateau->pions[i];
    }
    for (i = 0; i < nbFigures; i++) {
        plateauCopy->figures[i] = plateau->figures[i];
    }
    plateauCopy->temps = plateau->temps;
    return plateauCopy;
}


Etat Etat_init() {
    Etat plateauInit;
    plateauInit.temps = 0.;
    plateauInit.nbFigures = 0;
    plateauInit.nbPions = 0;
    plateauInit.points = 0.;
    return plateauInit;
}
void Etat_ajouterPion(Etat *plateau, Pos pion) {
    plateau->pions[plateau->nbPions] = pion;
    (plateau->nbPions)++;
}
void Etat_ajouterFigure(Etat *plateau, Pos figure) {
    plateau->figures[plateau->nbFigures] = figure;
    (plateau->nbFigures)++;
}

bool Etat_bougerPion(Etat *plateau, Pos pion, Pos dest) {
    bool trouve = false;
    int nbPions = plateau->nbPions, i;

    for (i = 0; i < nbPions; i++) {
        if (Pos_egal((plateau->pions[i]), pion) == true) {
            plateau->pions[i] = dest;
            trouve = true;
            break;
        }
    }

    return trouve;
}

bool Etat_bougerFigure(Etat *plateau, Pos figure, Pos dest) {
    bool trouve = false;
    int nbFigures = plateau->nbFigures, i;

    for (i = 0; i < nbFigures; i++) {
        if (Pos_egal((plateau->figures[i]), figure) == true) {
            plateau->figures[i] = dest;
            trouve = true;
            break;
        }
    }

    return trouve;
}

bool Etat_detruirePion(Etat *plateau, Pos pion) {
    bool trouve = false;
    int nbPions = plateau->nbPions, i, numPion;
    // trouver le pion à détruire
    for (i = 0; i < nbPions; i++) {
        if (Pos_egal((plateau->pions[i]), pion) == true) {
            numPion = i;
            trouve = true;
            plateau->nbPions--;
            break;
        }
    }
    // remettre les pions suivants aux bons rangs
    for (i = numPion + 1; i < nbPions; i++) {
        plateau->pions[i - 1] = plateau->pions[i];
    }

    return trouve;
}

bool Etat_detruireFigure(Etat *plateau, Pos figure) {
    bool trouve = false;
    int nbFigures = plateau->nbFigures, i, numFigure;
    // trouver la figure à détruire
    for (i = 0; i < nbFigures; i++) {
        if (Pos_egal((plateau->figures[i]), figure) == true) {
            numFigure = i;
            trouve = true;
            plateau->nbFigures--;
            break;
        }
    }
    // remettre les figures suivantes aux bons rangs
    for (i = numFigure + 1; i < plateau->nbPions; i++) {
        plateau->figures[i - 1] = plateau->figures[i];
    }

    return trouve;
}

/*Pos *Etat_copyPions(Etat *plateau) {
    int nbPions = plateau->nbPions, i;
    Pos *pions = (Pos *) malloc(nbPions * sizeof (Pos));
    for (i = 0; i < nbPions; i++) {
        pions[i] = plateau->pions[i];
    }
    return pions;
}

Pos *Etat_copyFigures(Etat *plateau) {
    int nbFigures = plateau->nbFigures, i;
    Pos *figures = (Pos *) malloc(nbFigures * sizeof (Pos));
    for (i = 0; i < nbFigures; i++) {
        figures[i] = plateau->figures[i];
    }
    return figures;
}*/


// ## Fonctions associées à la struct CoupPion

// Getters

Pos CoupPion_getPion(CoupPion *coup) {
    return coup->pion;
}

Pos CoupPion_getDest(CoupPion *coup) {
    return coup->dest;
}

Pos CoupPion_getRobot(CoupPion *coup) {
    return coup->robot;
}

// Setters

void CoupPion_setPion(CoupPion *coup, Pos pion) {
    coup->pion = pion;
}

void CoupPion_setDest(CoupPion *coup, Pos dest) {
    coup->dest = dest;
}

void CoupPion_setRobot(CoupPion *coup, Pos robot) {
    coup->robot = robot;
}

// ## Fonctions associées à la struct Tour

// Getters

Pos Tour_getFigure(Tour *tour) {
    return tour->figure;
}

Pos Tour_getPion1(Tour *tour) {
    return tour->pion1;
}

Pos Tour_getPion2(Tour *tour) {
    return tour->pion2;
}

// Setters

void Tour_setFigure(Tour *tour, Pos figure) {
    tour->figure = figure;
}

void Tour_setPion1(Tour *tour, Pos pion1) {
    tour->pion1 = pion1;
}

void Tour_setPion2(Tour *tour, Pos pion2) {
    tour->pion2 = pion2;
}


// ## Fonctions associées à la struct CoupTour

// Getters

Tour CoupTour_getTour(CoupTour *coup) {
    return coup->tour;
}

Pos CoupTour_getDest(CoupTour *coup) {
    return coup->dest;
}

Pos CoupTour_getRobot(CoupTour *coup) {
    return coup->robot;
}

// Setters

void CoupTour_setTour(CoupTour *coup, Tour tour) {
    coup->tour = tour;
}

void CoupTour_setDest(CoupPion *coup, Pos dest) {
    coup->dest = dest;
}

void CoupTour_setRobot(CoupTour *coup, Pos robot) {
    coup->robot = robot;
}

Pos calculerRobot(Pos pion, Pos dest) {
    float xP = pion.x, xD = dest.x, yP = pion.y, yD = dest.y;
    Pos robot;

    if (xP < xD + 0.5 && xP > xD - 0.5) { // le robot ne bouge pas en x depuis le pion
        robot.x = xP;

        if (yP < yD + 0.5 && yP > yD - 0.5) { // le robot ne bouge pas en y depuis le pion
            robot.y = pion.y; // (absurde)
        } else {
            if (yP <= yD - 0.5) { // le robot se déplace de bas en haut
                robot.y = yD - 0.5;
            } else { // le robot se déplace de haut en bas
                robot.y = yD + 0.5;
            }
        }

    } else {
        if (xP <= xD - 0.5) { // le robot se déplace de gauche à droite
            robot.x = xP - 0.5;

            if (yP < yD + 0.5 && yP > yD - 0.5) { // le robot ne bouge pas en y depuis le pion
                robot.y = pion.y; // (absurde)
            } else {
                if (yP <= yD - 0.5) { // le robot se déplace de bas en haut
                    robot.y = yD - 0.5;
                } else { // le robot se déplace de haut en bas
                    robot.y = yD + 0.5;
                }
            }

        } else { // le robot se déplace de droite à gauche
            robot.x = xP + 0.5;

            if (yP < yD + 0.5 && yP > yD - 0.5) { // le robot ne bouge pas en y depuis le pion
                robot.y = pion.y; // (absurde)
            } else {
                if (yP <= yD - 0.5) { // le robot se déplace de bas en haut
                    robot.y = yD - 0.5;
                } else { // le robot se déplace de haut en bas
                    robot.y = yD + 0.5;
                }
            }

        }
    }

    return robot;
}

// ## Fonctions associées à la struct Coup

// Getters

UCoup Coup_getUCoup(Coup *coup) {
    return coup->uCoup;
}

TCoup Coup_getTCoup(Coup *coup) {
    return coup->tCoup;
}
// Setters

void Coup_setUCoup(Coup *coup, UCoup uCoup) {
    coup->uCoup = uCoup;
}

void Coup_setTCoup(Coup *coup, TCoup tCoup) {
    coup->tCoup = tCoup;
}

// ## Fonctions pour interagir avec le plateau

float deciderCoupRec(Etat *plateau, float tempsLimite);

// deciderCoup :Décide d'un coup à jouer en fonction de l'Etat du plateau
// et de la profondeur limite déterminée par "dureeLimite"
// (analyse tous les coups possibles tant qu'ils prennent un temps total
//  inférieur à dureeLimite)
Coup deciderCoup(Etat *plateau, float dureeLimite) {
    int nbCoups, i, numCoupAJouer;
    float heuristique, heuristiqueMax, tempsLimite = dureeLimite + plateau->temps;
    Coup coupAJouer;
    Coup *coupsListe = coupsPossibles(plateau, &nbCoups);

    Etat plateauCopie = (*plateau);
    assert(jouerCoup(&plateauCopie, &(coupsListe[0])) == true);
    heuristiqueMax = deciderCoupRec(&plateauCopie, tempsLimite);
    numCoupAJouer = 0;
    for (i = 0; i < nbCoups; i++) {
        plateauCopie = (*plateau);
        assert(jouerCoup(&plateauCopie, &(coupsListe[i])) == true);
        heuristique = deciderCoupRec(&plateauCopie, tempsLimite);
        if (heuristique > heuristiqueMax) {
            numCoupAJouer = i;
            heuristiqueMax = heuristique;
        }
    }
    coupAJouer = coupsListe[numCoupAJouer];
    free(coupsListe);
    return (coupAJouer);
}


// Appel récursif initié par deciderCoup
float deciderCoupRec(Etat *plateau, float tempsLimite) { // tempsLimite = temps initial + dureeLimite
    if (plateau->temps >= tempsLimite || plateau->nbPions==0) {
        return (Etat_heuristique(plateau));
    }

    int nbCoups, i;
    float heuristique, heuristiqueMax;
    Coup *coupsListe = coupsPossibles(plateau, &nbCoups);

    Etat plateauCopie = (*plateau);
    assert(jouerCoup(&plateauCopie, &(coupsListe[0])) == true);
    heuristiqueMax = deciderCoupRec(&plateauCopie, tempsLimite);
    for (i = 1; i < nbCoups; i++) {
        plateauCopie = (*plateau);
        assert(jouerCoup(&plateauCopie, &(coupsListe[i])) == true);
        heuristique = deciderCoupRec(&plateauCopie, tempsLimite);
        if (heuristique > heuristiqueMax) {
            heuristiqueMax = heuristique;
        }
    }

    free(coupsListe);
    return heuristiqueMax;
}

bool Pos_estDansCaseEnnemie(Pos pion) {
    // Case ennemie  : x et y ont la même parité
    return (bool)(((int)(pion.x + 0.5) % 2) == ((int)(pion.y + 0.5) % 2));
}

Coup *coupsPossibles(Etat *plateau, int *nbCoups) {
    // Coups evalués : assembler une tour et la pousser dans une des 4 directions
    // ou pousser un pion d'une case dans une des quatres directions
    int i, nbPions = plateau->nbPions, nbCoupsMax = 4 * 16;
    Coup *coupsPossibles = (Coup *) malloc(sizeof (Coup) * nbCoupsMax);
    Coup coupAjoute;
    Tour tour;

    *nbCoups = 0;

    // Coups Pions
    coupAjoute.tCoup = PION;
    for (i = 0; i < nbPions; i++) {
        if (Pos_estDansCaseEnnemie(plateau->pions[i]) == true) {
            coupAjoute.uCoup.coupPion.robot = plateau->robotAmi;
            coupAjoute.uCoup.coupPion.pion = plateau->pions[i];

            // Pousser à gauche
            if ((plateau->pions[i]).x >= 0.5) { // le pion ne doit pas être au bord gauche
                coupAjoute.uCoup.coupPion.dest = plateau->pions[i];
                coupAjoute.uCoup.coupPion.dest.x = (float) floor(coupAjoute.uCoup.coupPion.dest.x + 0.5) - 1.;
                coupsPossibles[*nbCoups] = coupAjoute;
                (*nbCoups)++;
            }

            // Pousser à droite
            if ((plateau->pions[i]).x < 4.5) { // le pion ne doit pas être au bord droit
                coupAjoute.uCoup.coupPion.dest = plateau->pions[i];
                coupAjoute.uCoup.coupPion.dest.x = (float) floor(coupAjoute.uCoup.coupPion.dest.x + 0.5) + 1.;
                coupsPossibles[*nbCoups] = coupAjoute;
                (*nbCoups)++;
            }

            // Pousser en haut
            if ((plateau->pions[i]).y >= 0.5) { // le pion ne doit pas être au bord haut
                coupAjoute.uCoup.coupPion.dest = plateau->pions[i];
                coupAjoute.uCoup.coupPion.dest.y = (float) floor(coupAjoute.uCoup.coupPion.dest.y + 0.5) - 1.;
                coupsPossibles[*nbCoups] = coupAjoute;
                (*nbCoups)++;
            }

            // Pousser en bas
            if ((plateau->pions[i]).y < 4.5) { // le pion ne doit pas être au bord bas
                coupAjoute.uCoup.coupPion.dest = plateau->pions[i];
                coupAjoute.uCoup.coupPion.dest.y += (float) floor(coupAjoute.uCoup.coupPion.dest.y + 0.5) + 1.;
                coupsPossibles[*nbCoups] = coupAjoute;
                (*nbCoups)++;
            }
        }
    }

    // Coups Tour
    tour = deciderTour(plateau);
    if (tour.figure.x != -1) {
        coupAjoute.tCoup = TOUR;
        coupAjoute.uCoup.coupTour.robot = plateau->robotAmi;
        coupAjoute.uCoup.coupTour.tour = tour;
        if (Pos_estDansCaseEnnemie(tour.pion2) == true) { // On doit pousser la tour
            // Pousser à gauche
            if (tour.pion2.x >= 0.5) { // le pion ne doit pas être au bord gauche
                coupAjoute.uCoup.coupTour.dest = tour.pion2;
                coupAjoute.uCoup.coupTour.dest.x = (float) floor(coupAjoute.uCoup.coupTour.dest.x + 0.5) - 1.;
                coupsPossibles[*nbCoups] = coupAjoute;
                (*nbCoups)++;
            }

            // Pousser à droite
            if (tour.pion2.x < 4.5) { // le pion ne doit pas être au bord droit
                coupAjoute.uCoup.coupTour.dest = tour.pion2;
                coupAjoute.uCoup.coupTour.dest.x = (float) floor(coupAjoute.uCoup.coupTour.dest.x + 0.5) + 1.;
                coupsPossibles[*nbCoups] = coupAjoute;
                (*nbCoups)++;
            }

            // Pousser en haut
            if (tour.pion2.y >= 0.5) { // le pion ne doit pas être au bord haut
                coupAjoute.uCoup.coupTour.dest = tour.pion2;
                coupAjoute.uCoup.coupTour.dest.y = (float) floor(coupAjoute.uCoup.coupTour.dest.y + 0.5) - 1.;
                coupsPossibles[*nbCoups] = coupAjoute;
                (*nbCoups)++;
            }

            // Pousser en bas
            if (tour.pion2.y < 4.5) { // le pion ne doit pas être au bord bas
                coupAjoute.uCoup.coupTour.dest = tour.pion2;
                coupAjoute.uCoup.coupTour.dest.y += (float) floor(coupAjoute.uCoup.coupTour.dest.y + 0.5) + 1.;
                coupsPossibles[*nbCoups] = coupAjoute;
                (*nbCoups)++;
            }
        } else { // La tour est bien là où elle est (case amie)
            coupAjoute.uCoup.coupTour.dest = tour.pion2;
            coupsPossibles[*nbCoups] = coupAjoute;
            (*nbCoups)++;
        }
    }
    return coupsPossibles;
}

bool jouerCoup(Etat *plateau, Coup *coup) {
    bool valide;
    if (coup->tCoup == PION) {
        CoupPion *coupPion = &(coup->uCoup.coupPion);
        valide = jouerCoupPion(plateau, coupPion);
    } else { // TOUR
        CoupTour *coupTour = &(coup->uCoup.coupTour);
        valide = jouerCoupTour(plateau, coupTour);
    }
    return valide;
}

bool jouerCoupPion(Etat *plateau, CoupPion *coup) {
    Pos pion = coup->pion, dest = coup->dest;
    if (Etat_bougerPion(plateau, pion, dest) == false) {
        printf("FALSEPION\n");
        return false;
    }
    plateau->robotAmi = calculerRobot(pion, dest);
    plateau->temps += estimerTempsPion(plateau, coup);
    plateau->points += calculerPointsPion(&dest);

    return true;
}

bool jouerCoupTour(Etat *plateau, CoupTour *coup) {
    Pos figure = coup->tour.figure, pion1 = coup->tour.pion1;
    Pos pion2 = coup->tour.pion2, dest = coup->dest;
    if (Etat_detruireFigure(plateau, figure) == false) {
        printf("FALSEFIGURE\n");
        return false;
    }
    if (Etat_detruirePion(plateau, pion1) == false) {
        printf("FALSEPION1\n");
        return false;
    }
    if (Etat_detruirePion(plateau, pion2) == false) {
        printf("FALSEPION2\n");
        return true;
    }
    /*
    Pos figure = coup->tour.figure, pion1 = coup->tour.pion1;
    Pos pion2 = coup->tour.pion2, dest = coup->dest;
    if (Etat_bougerFigure(plateau, figure, dest) == false) {
        printf("FALSEFIGURE\n");
        return false;
    }
    if (Etat_bougerPion(plateau, pion1, dest) == false) {
        printf("FALSEPION1\n");
        return false;
    }
    if (Etat_bougerPion(plateau, pion2, dest) == false) {
        printf("FALSEPION2\n");
        return true;
    }*/

    plateau->robotAmi = calculerRobot(pion2, dest);
    plateau->temps += estimerTempsTour(plateau, coup);
    plateau->points += calculerPointsTour(&dest);

    return true;
}

/* float estimerTemps(Etat *plateau, Coup *coup) {
    float vitesseMoy = 1; // vitesse moyenne
    float tempsSup = 5; // temps supplémentaire, pour soulever la tour, la reposer...
    float distanceParcourue;
    Pos robot = plateau->robotAmi, pion1, pion2, figure, dest;

    if (coup->tCoup == PION) {
        pion1 = coup->uCoup->coupPion->pion;
        dest = coup->uCoup->coupPion->dest;
        distanceParcourue = Pos_distance(robot, pion1) +  Pos_distance(pion1, dest);
    } else { // TOUR
        pion1 = coup->uCoup->coupTour->tour->pion1;
        pion2 = coup->uCoup->coupTour->tour->pion2;
        figure = coup->uCoup->coupTour->tour->figure;
        distanceParcourue = Pos_distance(robot, pion1) + Pos_distance(pion1, pion2)
                            + tempsSup
                            + Pos_distance(pion2, figure) + Pos_distance(figure, dest);
    }

    return (distanceParcourue / vitesseMoy);
}*/

float estimerTempsPion(Etat *plateau, CoupPion *coup) {
    float vitesseMoy = VITESSE_MOY; // vitesse moyenne
    float distanceParcourue;
    Pos robot = plateau->robotAmi, pion, dest;

    pion = coup->pion;
    dest = coup->dest;
    distanceParcourue = Pos_distance(robot, pion) + Pos_distance(pion, dest);

    return (distanceParcourue / vitesseMoy);
}

float estimerTempsTour(Etat *plateau, CoupTour *coup) {
    float vitesseMoy = VITESSE_MOY; // vitesse moyenne
    float tempsSup = TEMPS_SUP; // temps supplémentaire : pour soulever la tour, la reposer, se tourner...
    float distanceParcourue;
    Pos robot = plateau->robotAmi, pion1, pion2, figure, dest;

    pion1 = coup->tour.pion1;
    pion2 = coup->tour.pion2;
    figure = coup->tour.figure;
    distanceParcourue = Pos_distance(robot, pion1) + Pos_distance(pion1, pion2)
            + Pos_distance(pion2, figure) + Pos_distance(figure, dest);


    return (distanceParcourue / vitesseMoy + tempsSup);
}

float Etat_heuristique(Etat *plateau) {
    return ((float) plateau->points / plateau->temps);
}

typedef struct Noeud_ {
    Pos assemblage[4]; /* Contient la position initiale du robot,
                        * puis des 3 pions ou figure à assembler */
    int nbPieces; /* Taille-1 du tableau "assemblage"
                   * Vaut 0, 1 (1 pion/figure choisi), 2 (2 pions/figure choisis) 
                   * ou 3 (2 pions + 1 figure choisis = 1 tour) */
    float cout; /* heuristique utilisée dans l'implémentation de A* */
} Noeud;

Noeud Noeud_init(Pos robot) { /* Constitue un noeud dansl'arbre de recherche de deciderTour */
    Noeud n;
    n.assemblage[0] = robot;
    n.nbPieces = 0;
    n.cout = 0;
    return n;
}

Noeud Noeud_ajoutePiece(Noeud noeud, Pos piece) {
    Noeud nouveauNoeud = noeud;
    float coutSup = Pos_distance(noeud.assemblage[noeud.nbPieces], piece);
    nouveauNoeud.nbPieces++;
    nouveauNoeud.assemblage[nouveauNoeud.nbPieces] = piece;
    nouveauNoeud.cout += coutSup;
    return nouveauNoeud;
}

Noeud *Noeud_trouveSuccesseurs(Noeud n, Etat *plateau, int *nbSuccesseurs) {
    int nbPieces = n.nbPieces;
    int i, numSuccesseur = 0;
    Noeud *successeurs;
    if (nbPieces > 3) {
        return 0;
    }
    if (nbPieces == 0) {
        *nbSuccesseurs = plateau->nbFigures; // Il faut chercher une figure
        successeurs = (Noeud *) malloc(sizeof(Noeud)*(*nbSuccesseurs));
        for (i = 0; i < *nbSuccesseurs; i++) {
            successeurs[i] = Noeud_ajoutePiece(n, plateau->figures[i]);
        }
    } else { // Il faut chercher un pion
        *nbSuccesseurs = plateau->nbPions - (nbPieces - 1);
        successeurs = (Noeud *) malloc(sizeof(Noeud) * (*nbSuccesseurs));

        if (nbPieces >= 2) { // Il y a un pion qui a déjà été choisi, il ne faut pas l'ajouter
            for (i = 0; i < plateau->nbPions; i++) {
                if (Pos_egal(plateau->pions[i], n.assemblage[nbPieces])) {
                    continue;
                }
                successeurs[numSuccesseur] = Noeud_ajoutePiece(n, plateau->pions[i]);
                numSuccesseur++;
            }
            //assert(numSuccesseur == (*nbSuccesseurs));
        } else { // Une figure a été choisie mais aucun pion : on ajoute tous les pions connus
            for (i=0; i < *nbSuccesseurs; i++) {
                successeurs[i] = Noeud_ajoutePiece(n, plateau->pions[i]);
            }
        }   
    }
    return successeurs;
}

typedef struct TasNoeud_ { // File de priorité implémentée en tas
    Noeud noeuds[15 * 14 * 4]; /* Taille maximale de la pile
                               * nbTourPossibles = 15pions*14pions*4figures */
    int nbNoeuds; /* Nombre de noeuds actuel du tas */
} TasNoeud;

TasNoeud TasNoeud_init(Noeud nInit) {
    TasNoeud tas;
    tas.noeuds[0] = nInit;
    tas.nbNoeuds = 1;
    return tas;
}

#define GAUCHE(k)           (2*k)
#define DROIT(k)            (2*k+1)
#define PARENT(k)           (k/2)
#define ESTVIDE(tas,k)      (k >= tas->nbNoeuds)
#define ECHANGER(tas, i,j)  { Noeud x = tas->noeuds[i]; \
                              tas->noeuds[i] = tas->noeuds[j]; \
                              tas->noeuds[j] = x; }

void TasNoeud_descendre(TasNoeud *tas, int k) {
    int maxcout, droit, gauche;
    gauche = GAUCHE(k);
    droit = DROIT(k);
    //if (!ESTVIDE(tas, k)) { // Condition de terminaison
    if (!ESTVIDE(tas, gauche)) { // Condition de terminaison
        maxcout = gauche;
        if (!ESTVIDE(tas, droit)) {
            if (tas->noeuds[droit].cout > tas->noeuds[maxcout].cout) {
                maxcout = droit;
            }
        }
        if (tas->noeuds[k].cout > tas->noeuds[maxcout].cout) { // Autre condition de terminaison
            ECHANGER(tas, k, maxcout);
            TasNoeud_descendre(tas, maxcout); // Continuer tant que le tas n'est pas ordonné
        }
    }
}

void TasNoeud_monter(TasNoeud *tas, int k) {
    int mincout, parent;
    if (k > 0) { // Condition de terminaison
        parent = PARENT(k);
        mincout = parent;
        if (tas->noeuds[k].cout < tas->noeuds[mincout].cout) { // Autre condition de terminaison
            ECHANGER(tas, k, mincout);
            TasNoeud_monter(tas, mincout); // Continuer tant que le tas n'est pas ordonné
        }
    }
}
/*
void TasNoeud_ajouter(TasNoeud *tas, Noeud n) {
    assert(tas != NULL);
    assert(tas->nbNoeuds <= 15 * 14 * 4);
    // Incrémenter le nombre de noeuds et ajouter n en bas du tas
    tas->noeuds[tas->nbNoeuds] = n;
    tas->nbNoeuds++;
    // Permuter le noeud ajouter avec les parents de pire coût
    TasNoeud_monter(tas, tas->nbNoeuds-1);
    //TasNoeud_monter(tas, tas->nbNoeuds);
}*/

void TasNoeud_ajouter(TasNoeud *tas, Noeud n) {
    assert(tas != NULL);
    assert(tas->nbNoeuds <= 15 * 14 * 4);
    // Incrémenter le nombre de noeuds et ajouter n en bas du tas
    tas->noeuds[tas->nbNoeuds] = n;
    tas->nbNoeuds++;
}

/*
Noeud TasNoeud_retirer(TasNoeud * tas) {
    int nbNoeuds = tas->nbNoeuds;
    Noeud racine;
    assert(tas != NULL);
    assert(nbNoeuds >= 1);
    // Garder la racine (élément de moindre cout)
    racine = tas->noeuds[0];
    // Mettre le dernier élément comme racine
    tas->noeuds[0] = tas->noeuds[nbNoeuds - 1];
    // Décrémenter le nb de noeuds contenus dans tas
    tas->nbNoeuds--;
    // Permuter le noeud racine avec les fils de meilleur coût afin de réordonner le tas
    TasNoeud_descendre(tas, 1);
    return racine;
}*/

Noeud TasNoeud_retirer(TasNoeud * tas) {
    int i, numMin, nbNoeuds = tas->nbNoeuds;
    Noeud min;
    assert(tas != NULL);
    assert(nbNoeuds >= 1);

    numMin = 0;
    min = tas->noeuds[0];
    for (i = 1; i < nbNoeuds; i++) {
        if (tas->noeuds[i].cout < min.cout) {
            numMin = i;
            min = tas->noeuds[i];
        }
    }
    
    for (i = numMin; i < nbNoeuds -1; i++) {
        tas->noeuds[i] = tas->noeuds[i+1];
    }

    (tas->nbNoeuds)--;
    return min;
}

Tour deciderTour(Etat *plateau) { /* Utilise l'algorithme de recherche coût uniforme */
    Tour solution;

    if (plateau->nbFigures == 0 || plateau->nbPions < 2) {
        solution.figure.x = -1; // signe qu'aucune tour n'a pu être assemblée
        return solution;
    }

    bool succes = false;
    Noeud n = Noeud_init(plateau->robotAmi);
    TasNoeud frontiere = TasNoeud_init(n);
    int i;

    while (succes == false) {
        assert(frontiere.nbNoeuds != 0);

        n = TasNoeud_retirer(&frontiere);
        int nbPieces = n.nbPieces;
        if (nbPieces == 3) {
            succes = true;
        } else {
            int nbSuccesseurs;
            Noeud *successeurs = Noeud_trouveSuccesseurs(n, plateau, &nbSuccesseurs);
            for (i = 0; i < nbSuccesseurs; i++) {
                TasNoeud_ajouter(&frontiere, successeurs[i]);
            }
            free(successeurs);
        }
    }

    solution.figure = n.assemblage[1];
    solution.pion1 = n.assemblage[2];
    solution.pion2 = n.assemblage[3];
    return solution;
}

