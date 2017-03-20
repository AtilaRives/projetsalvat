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

#define TAILLE_TAB 2048
#define TAILLE_NOM 512
#define TAILLE_DESTINATION 20
#define MAX_PROFONDEUR 1000


/*STRUCTURE*/
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

struct str_arcs_transfert{
    struct str_arrets* to_stop_id;
    int temps_trajet;
    int transfer_type;//-1 si c'est un arcs
};


/*TYPEDEF*/
typedef struct str_arrets str_arrets;
typedef struct str_arcs_transfert str_arcs_transfert;
typedef struct str_noeud str_noeud;

struct str_noeud{
    str_arrets *ptr_vers_arret;
    struct str_noeud *papa;
    struct str_noeud *next;
};


/*DECLARATION VARIABLES*/
str_arrets tableau_d_arret[TAILLE_TAB];


/*DECLARATION FUNCTION*/
int fnc_remplir_arret(FILE *arret);
void fnc_innit_tableau();
int fnc_remplir_trajet(FILE *arcs, FILE *transferts,int nombre_d_arret);
void fnc_station_vers_leurs_double(int nombre_d_arret);
str_noeud *link_dad_son(str_noeud *dad, int cpt); /*A ajouter dans .C*/
void print_arret(str_arrets * arret); /*A ajouter dans .C*/
str_arrets *arret_from_id(int id, int nombre_arret); /*A ajouter dans .C*/
int launch_p_prof(int depard, int arrivee, int nombre_arret, int profondeur_max);/*A ajouter dans .C*/

#endif /* header_h */
