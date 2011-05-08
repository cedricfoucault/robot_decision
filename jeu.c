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

float Pos_distance(Pos *pos1, Pos *pos2) {
    float x1 = pos1->x, y1 = pos1->y, x2 = pos2->x, y2 = pos2->y;
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

boolean Pos_egal(Pos *pos1, Pos *pos2) {
    if ((pos1->x == pos2->x) && (pos1->y == pos2->y)) {
        return TRUE;
    } else {
        return FALSE;
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

Pos Etat_getRobotEnnemi(Etat *plateau) {
    return plateau->robotEnnemi;
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

void Etat_setRobotEnnemi(Etat *plateau, Pos robotEnnemi) {
    plateau->robotEnnemi = robotEnnemi;
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
    Etat *plateauCopy = (Etat *) malloc(sizeof (Etat));
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

boolean Etat_bougerPion(Etat *plateau, Pos pion, Pos dest) {
    boolean trouve = FALSE;
    int nbPions = plateau->nbPions, i;

    for (i = 0; i < nbPions; i++) {
        if (Pos_egal((plateau->pions[i]), pion) == TRUE) {
            plateau->pions[i] = dest;
            trouve = TRUE;
            break;
        }
    }

    return trouve;
}

boolean Etat_bougerFigure(Etat *plateau, Pos figure, Pos dest) {
    boolean trouve = FALSE;
    int nbFigures = plateau->nbFigures, i;

    for (i = 0; i < nbFigures; i++) {
        if (Pos_egal((plateau->figures[i]), figure) == TRUE) {
            plateau->figures[i] = dest;
            trouve = TRUE;
            break;
        }
    }

    return trouve;
}

boolean Etat_detruirePion(Etat *plateau, Pos pion) {
    boolean trouve = FALSE;
    int nbPions = plateau->nbPions, i, numPion;
    // trouver le pion à détruire
    for (i = 0; i < nbPions; i++) {
        if (Pos_egal((plateau->pions[i]), pion) == TRUE) {
            numPion = i;
            trouve = TRUE;
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

boolean Etat_detruireFigure(Etat *plateau, Pos figure) {
    boolean trouve = FALSE;
    int nbFigures = plateau->nbFigures, i, numFigure;
    // trouver la figure à détruire
    for (i = 0; i < nbFigures; i++) {
        if (Pos_egal((plateau->figures[i]), figure) == TRUE) {
            numFigure = i;
            trouve = TRUE;
            plateau->figures--;
            break;
        }
    }
    // remettre les figures suivantes aux bons rangs
    for (i = numFigure + 1; i < nbPions; i++) {
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
    coup->tour = robot;
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

Coup deciderCoup(Etat *plateau) {

}

Coup coupsPossibles(Etat *plateau) {

}

boolean jouerCoup(Etat *plateau, Coup *coup) {
    boolean valide;
    if (coup->tCoup == PION) {
        valide = jouerCoupPion(plateau, coup->uCoup->coupPion);
    } else { // TOUR
        valide = jouerCoupTour(plateau, coup->uCoup->coupTour);
    }
    return valide;
}

boolean jouerCoupPion(Etat *plateau, CoupPion *coup) {
    Pos pion = coup->pion, dest = coup->dest;
    if (Etat_bougerPion(plateau, pion, dest) == FALSE) {
        return FALSE;
    }
    plateau->robotAmi = calculerRobot(pion, dest);
    plateau->temps += estimerTempsPion(plateau, coup);
    plateau->points += calculerPointsPion(dest);

    return TRUE;
}

boolean jouerCoupTour(Etat *plateau, CoupTour *coup) {
    Pos figure = coup->tour->figure, pion1 = coup->tour->pion1;
    Pos pion2 = coup->tour->pion2, dest = coup->dest;
    if (Etat_bougerFigure(plateau, figure, pion1) == FALSE) {
        return FALSE;
    }
    if (Etat_bougerPion(plateau, pion1, pion2) == FALSE) {
        return FALSE;
    }
    if (Etat_bougerFigure(plateau, figure, pion2) == FALSE) {
        return FALSE;
    }
    if (Etat_bougerPion(plateau, pion1, dest) == FALSE) {
        return FALSE;
    }
    if (Etat_bougerFigure(plateau, figure, dest) == FALSE) {
        return FALSE;
    }
    if (Etat_bougerPion(plateau, pion2, dest) == FALSE) {
        return FALSE;
    }
    plateau->robotAmi = calculerRobot(pion2, dest);
    plateau->temps += estimerTempsTour(plateau, coup);
    plateau->points += calculerPointsTour(dest);

    return TRUE;
}

boolean coupValide(Etat *plateau, Coup *coup) {

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
    float vitesseMoy = 1; // vitesse moyenne
    float distanceParcourue;
    Pos robot = plateau->robotAmi, pion, dest;

    pion = coup->pion;
    dest = coup->dest;
    distanceParcourue = Pos_distance(robot, pion) + Pos_distance(pion, dest);

    return (distanceParcourue / vitesseMoy);
}

float estimerTempsTour(Etat *plateau, CoupTour *coup) {
    float vitesseMoy = 1; // vitesse moyenne
    float tempsSup = 5; // temps supplémentaire : pour soulever la tour, la reposer, se tourner...
    float distanceParcourue;
    Pos robot = plateau->robotAmi, pion1, pion2, figure, dest;

    pion1 = coup->tour->pion1;
    pion2 = coup->tour->pion2;
    figure = coup->tour->figure;
    distanceParcourue = Pos_distance(robot, pion1) + Pos_distance(pion1, pion2)
            + tempsSup
            + Pos_distance(pion2, figure) + Pos_distance(figure, dest);


    return (distanceParcourue / vitesseMoy);
}

float heuristique(Etat *plateau) {
    return ((float)plateau->points / plateau->temps);
}

Tour deciderTour(Etat *plateau) {
    
}