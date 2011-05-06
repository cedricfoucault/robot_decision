/*
 * File:   io.h
 * Author: Cedric
 *
 * Created on 6 mai 2011, 00:00
 */


#ifndef IO_H
#define	IO_H

#ifdef	__cplusplus
extern "C" {
#endif

    // INPUT OUTPUT
    // ------------
    //
    // Interface pour que la procédure maîtresse du robot puisse communiquer
    // avec le module de prise de décision.


    // ### APPROCHE
    //
    // On admet que la procedure maitresse du programme (le "main") fait appel au
    // module de prise de decision de maniere ponctuelle, par exemple :
    // 1. querir une decision d'acte
    // 2. executer l'action; si l'action est impossible a executer, goto 1.
    // 3. une fois l'action executee, goto 1.
    // De cette facon, on simplifie considerablement l'IA puisqu'il ne s'agit plus
    // que de traiter des etats instantanes.


    // ### TRAITEMENT INPUT
    //
    // Cette partie a pour rôle de fournir des informations sur l'environnement de
    // jeu recueillies par le robot au module de prise de décision.
    // On suppose que ce module a correctement abstrait ces informations pour
    // pouvoir fourni par exemple la position d'un palet, ou d'un robot.

    void mettreAJourEtat(void *donnees);
    /* met à jour l'état du plateau de jeu virtuel
     * en fonction des données recueillies par le robot
     * (detection d'un palet, déplacement du robot ...)*/

    // ### TRAITEMENT OUTPUT
    //
    // Cette partie fait appel au module de prise de décision pour décider
    // d'un coup à jouer.
    // Le coup à jouer doît ensuite converti en informations que le robot
    // peut traiter (une action) en vue de déplacer un palet, assembler une tour...
    //
    // NOTE : il est necessaire de determiner les actions "atomiques" du robot
    // (aller au point A, saisir, lacher, pousser, etc) afin d'etablir une
    // convention de communication avec la procedure maitresse.

    typedef Action;
    Action querirAction();

#ifdef	__cplusplus
}
#endif

#endif	/* IO_H */

