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
#define DISTANCE_MAX 999999

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

struct str_noeud{
    struct str_arrets *ptr_vers_arret;
    struct str_noeud *papa;
    struct str_noeud *next;
};

struct arret2{
    //doublement chainée (pas forcement utilisé, mais ce sera fait)
    struct arret2 *next;
    struct str_arrets *ptr_vers_arret;
    struct arret2 *papa;
    int distance_totale;
    int arret_bool; //0non traité, 1 traité.
};


/*TYPEDEF*/
typedef struct str_arrets str_arrets;
typedef struct str_arcs_transfert str_arcs_transfert;
typedef struct str_noeud str_noeud;


typedef struct arret2 arret2;



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
/*parcours Dijkstra*/
void innit_arret2(arret2 *ancre, int nb_arret);
int parcour_dijkstra(long depart, long arrivee, int nombre_arret);
void fnc_find_min(arret2 *premier_arret2, arret2 *arret2_return);
void bool_to_true(long id, arret2 *current);
int parcour_done(const arret2 *ancre);
arret2* find_the_smallest_one(const arret2 *ancre);
void find_and_parse_neighborhood(arret2 *current, const arret2 *ancre);


#endif /* header_h */
