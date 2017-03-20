//
//  main.c
//  parcours_en_profondeur
//
//  Created by Pichot on 20/03/2017.
//  Copyright � 2017 Pichot. All rights reserved.
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
        printf("Probl�me ouverture fichier arcs\n");
        return -14;
    }

    file_transferts = fopen("C:\\Users\\atila\\Documents\\AlgoRATP\\transferts.txt", "r");
    if (file_transferts == NULL)
    {
        printf("Probl�me ouverture fichier transferts\n");
        return -13;
    }

    file_arrets = fopen("C:\\Users\\atila\\Documents\\AlgoRATP\\arrets.txt", "r");
    if (file_arrets == NULL)
    {
        printf("Probl�me ouverture fichier arrets\n");
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

    /*Cr�ation des doubles*/
    fnc_station_vers_leurs_double(nombre_arret);

    /*Fermeture files*/
    fclose(file_arcs);
    fclose(file_arrets);
    fclose(file_transferts);



//        for(int i =0;i<nombre_arret-10;i++)
//    {
//
////        printf("%ld %d\n",tableau_d_arret[i].stop_id, tableau_d_arret[i].i_station);
//            print_arret(&tableau_d_arret[i]);
//
//    }




    /*Appel fonction parcours en profondeur*/
   launch_p_prof(2390, 2453, nombre_arret, profondeur_max);
   // launch_p_prof(4799027, 6046800, nombre_arret, profondeur_max);

    printf("Fin du main\n");

    return 0;
}


int parcours_profondeur(str_arrets *depart , str_arrets *arrive, int result, int profondeur, int *cpt_noeud, int max_profondeur){


    /*D�claration working data*/
    int compteur = 0;

    /*Incr�mentation Profondeur*/
    profondeur++;

    /*Test resultat trouv�*/
    if(depart == arrive){
        printf("Il y a eu en tout %d noeuds\n", cpt_noeud);
        printf("%d  ",profondeur);
        /*Affichage du resultat*/
        print_arret(depart);

        return 1;
    }



    /*Boucle de parcours des fils*/
    while(/*Test de profondeur*/profondeur < max_profondeur &&/*Test bool*/result == 0 &&/*Test enfant*/depart->i_station >= compteur){

        /*Accr�mentation du int * afin de pouvoir remonter info*/
        cpt_noeud++;

        /*Appel r�curssif*/
        result = parcours_profondeur(depart->stations_suivantes[compteur].to_stop_id, arrive, result, profondeur, cpt_noeud, max_profondeur);
        compteur++;
         if (result == 1)
        {
            printf("%d  ",profondeur);
            print_arret(depart);
            return result;
        }
    }



//    printf("id current %ld\n", depart->stop_id);
//if(profondeur==1000) {printf("id current %ld\n", depart->stop_id);}

    return result;
}

int launch_p_prof(int depart, int arrivee, int nombre_arret, int profondeur_max){

    /*Declaration working data*/
    int *cpt_noeud = 0;
    int profondeur = 0;
    str_arrets *departi = NULL;
    str_arrets *arriveei = NULL;


    /*Init struct arret from ID*/
    departi = arret_from_id(depart, nombre_arret);
    arriveei = arret_from_id(arrivee, nombre_arret);

    printf(" arrive : %d,  depart %d\n", departi->i_station,  arriveei->i_station);
    parcours_profondeur(departi, arriveei, 0, profondeur, cpt_noeud, profondeur_max);

    return 0;
}


str_arrets *arret_from_id(int id, int nombre_arret){

    int i =0;
    str_arrets *res;

    while( id != tableau_d_arret[i].stop_id)
    {
        i++;
        if(i>nombre_arret-2){
            printf("On a pas trouv� l'arret\n");
            exit(-1);
        }
    }
    return res = &tableau_d_arret[i];
}

str_noeud *link_dad_son(str_noeud *dad, int cpt){

    /*Init*/
    str_noeud *fils = NULL;
    fils = malloc(sizeof(str_noeud));

    /*Test Malloc*/
    if(fils==NULL){
        printf("Error malloc sur fils\n");
        exit(-1);
    }

    /*Linkage*/
    fils->papa = dad;
    fils->ptr_vers_arret=dad->ptr_vers_arret->stations_suivantes[cpt].to_stop_id;
    fils->next=NULL;

    return fils;
}



/*FUNCTION*/

void print_arret(str_arrets * arret){
    printf("%ld %s %s %f %f \n%d\n\n", arret->stop_id, arret->stop_name, arret->stop_desc,
           arret->stop_lat , arret->stop_lon, arret->i_station);
}




struct arret2{
    long id;
    struct arret2 *predecesseur;
    int distance_totale;
    int arret_bool;
};
typedef struct arret2 arret2;

struct saladier{
    struct saladier *suivant;
    struct saladier *precedent;
    struct arret2 *arret;
};
typedef struct saladier saladier;
void innit_arret2(saladier *conteneur, int nb_arret){
    int i = 0;
    conteneur->precedent=NULL;
    conteneur->arret = NULL;
    conteneur->suivant = NULL;
    conteneur->arret = malloc(sizeof(arret2));
    if (conteneur->arret==NULL){
        printf("Erreur de malloc sur arret2");
        exit(-3);
    }
    conteneur->arret->distance_totale=0;
    conteneur->arret->id=-1;
    conteneur->arret->arret_bool=0;

    saladier *conteneur_buffer=NULL;

    while(i < nb_arret-2){
        i++;
        //chainage double;
        conteneur_buffer = conteneur;
        conteneur=conteneur->suivant;
        conteneur->precedent=conteneur_buffer;
        conteneur->suivant = NULL;
        conteneur->arret = malloc(sizeof(arret2));
        if (conteneur->arret==NULL){
            printf("Erreur de malloc sur arret2");
            exit(-3);
        }
        conteneur->arret->distance_totale=-1;
        conteneur->arret->id=-1;
        conteneur->arret->arret_bool=0;
    }

}

void launch_parcour_dijkstra(long depart, long arrivee, int nombre_arret){
    str_arrets *departi = NULL;
    str_arrets *arriveei = NULL;

    departi = arret_from_id(depart, nombre_arret);
    arriveei = arret_from_id(arrivee, nombre_arret);

    parcour_dijkstra(departi, arriveei);
}
int parcour_dijkstra(arret2 *depart, arret2 *arrivee, int nombre_arret){
    saladier *conteneur = NULL;

   innit_arret2(conteneur, nombre_arret);

}
