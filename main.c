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

    int profondeur_max;
    int i = 0;
    int nombre_arret;

    /* mise en argument de la profondeur MAX */
    /* Test argument*/
    if(argc != 2){
      //  printf("Usage : Profondeur MAX\n");
        profondeur_max = 10;
        //return -1;
    }


    /* Stock argument */
    //profondeur_max = atoi(argv[1]);

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

   // fnc_afficher_destination(nombre_arret);
    /*Appel fonction parcours en profondeur*/
   //launch_p_prof(2390, 2453, nombre_arret, profondeur_max);
   // launch_p_prof(4799027, 6046800, nombre_arret, profondeur_max);

    parcour_dijkstra(2390, 2453, nombre_arret);

    printf("Fin du main\n");

    return 0;
}






void fnc_afficher_destination(int nb_arret){
    for (int i=0; i<nb_arret-10; i++){
        printf("%ld\n", tableau_d_arret[i].stop_id);
        for(int j = 0; j<= tableau_d_arret[i].i_station; j++){
            printf("   %ld   ",tableau_d_arret[i].stations_suivantes[j].to_stop_id->stop_id);
            printf("%d\n",tableau_d_arret[i].stations_suivantes[j].temps_trajet);
        }
    }
}
