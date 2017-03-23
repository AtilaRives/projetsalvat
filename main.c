//
//  main.c
//  parcours_en_profondeur
//
//  Created by Pichot on 20/03/2017.
//  Copyright © 2017 Pichot. All rights reserved.
//

#include <stdio.h>
#include "header.h"


int main(int argc, const char * argv[]) {

    int profondeur_max = 0;
    int i = 0;
    int nombre_arret;
    int *cpt_de_noeud = NULL;

    cpt_de_noeud = malloc(sizeof(int));
    if(cpt_de_noeud==NULL){
        printf("Probleme avec le malloc sur le compteur de noeuds\n");
        exit(-42);
    }
    *cpt_de_noeud=0;

    /* mise en argument de la profondeur MAX */
    /* Test argument*/
    if(argc != 2){
      //  printf("Usage : Profondeur MAX\n");
        profondeur_max = 10;
        //return -1;
    }
    else{
        profondeur_max == atoi(argv[1]);
    }


    /* Stock argument */


    fnc_innit_tableau();

    /*Ouverture files*/
    FILE* file_arcs= NULL;
    FILE* file_arrets = NULL;
    FILE* file_transferts = NULL;



    file_arcs = fopen("C:\\Users\\atila\\Documents\\AlgoRATP\\arcs.txt", "r");
    if (file_arcs == NULL)
    {
        printf("Problème ouverture fichier arcs\n");
        return -14;
    }

    file_transferts = fopen("C:\\Users\\atila\\Documents\\AlgoRATP\\transferts.txt", "r");
    if (file_transferts == NULL)
    {
        printf("Problème ouverture fichier transferts\n");
        return -13;
    }

    file_arrets = fopen("C:\\Users\\atila\\Documents\\AlgoRATP\\arrets.txt", "r");
    if (file_arrets == NULL)
    {
        printf("Problème ouverture fichier arrets\n");
        return -12;
    }


    fnc_remplir_arret(file_arrets);
    /*Compteur nombre d'arret*/
    while(tableau_d_arret[i].stop_id!=(-1) && i<TAILLE_TAB)
    {
        i++;
    }
    nombre_arret = i;

    /*Remplissage trajet*/
    fnc_remplir_trajet(file_arcs, file_transferts, nombre_arret);

    /*Création des doubles*/
    fnc_station_vers_leurs_double(nombre_arret);

    /*Fermeture files*/
    fclose(file_arcs);
    fclose(file_arrets);
    fclose(file_transferts);


    /*Appel fonctions*/


    the_compteur=0;
   *cpt_de_noeud=0;
   printf("DEBUT PARCOUR LARGEUR\n-----------------------------------\n");
   fnc_chercher_une_destination(2313, 2389, nombre_arret, cpt_de_noeud);
   printf("Nombre de noeuds parcourus %d\n\n\n", *cpt_de_noeud);


   *cpt_de_noeud=0;
   printf("DEBUT PARCOUR PROFONDEUR AVEC LIMITE A 30\n-----------------------------------\n");
   launch_p_prof(2313, 2389, nombre_arret, 30, cpt_de_noeud);
   printf("Nombre de noeuds parcourus %d  <- valeur non coherentes\n\n\n", *cpt_de_noeud);
   printf("Un compteur en global %d\n", the_compteur);


   *cpt_de_noeud=0;
   printf("DEBUT PARCOUR PROFONDEUR INCREMENTALE\n-----------------------------------\n");
   parcours_profondeur_borne(2313, 2389, nombre_arret, 1, cpt_de_noeud);
   printf("Nombre de noeuds parcourus %d\n\n\n", *cpt_de_noeud);



   *cpt_de_noeud=0;
   printf("DEBUT PARCOUR DIJKSTRA\n-----------------------------------\n");
   parcour_dijkstra(2313, 2389, nombre_arret, cpt_de_noeud);
   printf("Nombre de noeuds parcourus %d\n", cpt_de_noeud);
   printf("Et encore, on s'arrete quand le i min est infini, sinon il faudrait encore parcourir tous nos sommets\n\n", *cpt_de_noeud);


   *cpt_de_noeud=0;
    printf("DEBUT PARCOUR GLOUTON\n-----------------------------------\n");
    parcour_glouton(2313,2389,nombre_arret, cpt_de_noeud);
    printf("Nombre de noeuds parcourus %d\n\n\n", *cpt_de_noeud);




    printf("END %d\n");

    return 0;
}

