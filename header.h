//
//  header.h
//  parcours_en_profondeur
//
//  Created by Pichot on 20/03/2017.
//  Copyright © 2017 Pichot. All rights reserved.
//

#ifndef header_h
#define header_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "distance_log_lat.h"

#define TAILLE_TAB 2048
#define TAILLE_NOM 512
#define TAILLE_DESTINATION 20
#define MAX_PROFONDEUR 1000
#define DISTANCE_MAX 999999
#define VALEUR_D_AUGMENTATION 1


int the_compteur;

/*STRUCTURE*/

/** str_arrets represente un arret (ou une station)
* il est composé des mêmes éléments que dans le fichier arret
*
* \param i_station represente le nombre de destination -1 s'il n'y en a aucune est 0 pour 1, 1 pour 2 ...
* \param stations_suivantes represente un trajet d'un arret A vers un arret B
*/
struct str_arrets{
    long stop_id;
    //int stop_code;
    char stop_name[TAILLE_NOM];
    char stop_desc[TAILLE_NOM];
    double stop_lat;
    double stop_lon;
    //int location_type;
    //int parent_station;
    struct str_arcs_transfert *stations_suivantes;
    int i_station;

};

/** str_arcs_transfert est contenue dans une station depart
* \param to_stop_id la station arrivee
* \param transfer_type n'est pas utilisé dans le programme actuellement. Innitialiser a -1 pour les arcs et -2 pour les doublons (changement de voie)
*/
struct str_arcs_transfert{
    struct str_arrets* to_stop_id;
    int temps_trajet;
    int transfer_type;//-1 si c'est un arcs
};


/** str_noeud est utilisé pour representer les graphes dans la recherche en profondeur et largeur
* \param ptr_vers_arret est un pointeur vers une str_arrets contenue dans le tableau_d_arret (en global)
*/
struct str_noeud{
    struct str_arrets *ptr_vers_arret;
    struct str_noeud *papa;
    struct str_noeud *next;
};

/** arret2 est utilisé pour le parcours de Dijkstra et de Glouton
* il contient deux élément supplémentaire (on aurait du utiliser "améliorer" la str_noeud au lieu de creer arret2)
* \param distance_totale est utilisé avec le temps de trajet dans le parcour de Dijkstra et avec la distance "à vol d'oiseau" calculé grace à
* la longitute et latitute dans Glouton
* \param arret_bool innitialiser à 0, comme nous n'avons pas "retirer" l'élèment quand nous l'avons traité, nous utilisons un boolean pour le simuler (1 quand traité)
*/
struct arret2{
    //doublement chainée (pas forcement utilisé, mais ce sera fait)
    struct arret2 *next;
    struct str_arrets *ptr_vers_arret;
    struct arret2 *papa;
    double distance_totale;
    int arret_bool; //0non traité, 1 traité.
};


/*TYPEDEF*/
typedef struct str_arrets str_arrets;
typedef struct str_arcs_transfert str_arcs_transfert;
typedef struct str_noeud str_noeud;
typedef struct arret2 arret2;



/*DECLARATION VARIABLES*/
/**
* tableau contenant tous les str_arrets innitiaux et leurs valeurs.
*/
str_arrets tableau_d_arret[TAILLE_TAB];


/*DECLARATION FUNCTION*/
void print_arret(str_arrets * arret);


/* Largeurs */

void fnc_chercher_une_destination(long id_station_depard, long id_station_arrivee, int NOMBRE_D_ARRET, int *cpt_de_noeud);
int fnc_parcour_en_largeur(str_arrets *depard, str_arrets *arrive, int *cpt_de_noeud);
str_noeud * fnc_noeud_suivant(str_noeud * noeud_enfant, str_noeud * noeud_papa, int cpt_enfant);



/*Profondeur*/
int parcours_profondeur(str_arrets *depart , str_arrets *arrive, int result, int profondeur, int *cpt_noeud, int max_profondeur);
int fnc_remplir_arret(FILE *arret);
void fnc_innit_tableau();
int fnc_remplir_trajet(FILE *arcs, FILE *transferts,int nombre_d_arret);
void fnc_station_vers_leurs_double(int nombre_d_arret);
str_noeud *link_dad_son(str_noeud *dad, int cpt, int *cpt_noeud);
str_arrets *arret_from_id(int id, int nombre_arret);
int launch_p_prof(int depard, int arrivee, int nombre_arret, int profondeur_max, int *cpt_noeud);

void parcours_profondeur_borne(int depard, int arrivee, int nombre_arret, int profondeur_max, int *cpt_noeud);

/*parcours Dijkstra*/
void innit_arret2(arret2 *ancre, int nb_arret);
int parcour_dijkstra(long depart, long arrivee, int nombre_arret, int *cpt_de_noeud);
void fnc_find_min(arret2 *premier_arret2, arret2 *arret2_return);
int parcour_done( arret2 *ancre);
arret2* find_the_smallest_one( arret2 *ancre, int *cpt_de_noeud);
arret2* find_and_parse_neighborhood(arret2 *current,  arret2 *ancre, long arrivee, int *cpt_de_noeud);
double fnc_distance_entre_point(str_arrets *arret_a, str_arrets *arret_b);

/*parcours glouton*/
int parcour_glouton(long depart, long arrivee, int nombre_arret, int *cpt_de_noeud);
arret2 *innit_glouton(arret2 *ancre, long depart, long arrivee, int nombre_arret);
arret2 *find_neighborhood_glouton(arret2 *current, arret2 *ancre, long arrivee, int nombre_arret, int *cpt_de_noeud);
arret2 *double_to_arret2(long in, arret2 *ancre, int nombre_arret);


#endif /* header_h */
