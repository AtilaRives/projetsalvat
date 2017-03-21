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


    /*Déclaration working data*/
    int compteur = 0;

    /*Incrémentation Profondeur*/
    profondeur++;

    /*Test resultat trouvé*/
    if(depart == arrive){
        printf("Il y a eu en tout %d noeuds\n", cpt_noeud);
        printf("%d  ",profondeur);
        /*Affichage du resultat*/
        print_arret(depart);

        return 1;
    }



    /*Boucle de parcours des fils*/
    while(/*Test de profondeur*/profondeur < max_profondeur &&/*Test bool*/result == 0 &&/*Test enfant*/depart->i_station >= compteur){

        /*Accrémentation du int * afin de pouvoir remonter info*/
        cpt_noeud++;

        /*Appel récurssif*/
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
            printf("On a pas trouvé l'arret\n");
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
    //doublement chainée (pas forcement utilisé, mais ce sera fait)
    struct arret2 *next;
    str_arrets *ptr_vers_arret;
    struct arret2 *papa;
    int distance_totale;
    int arret_bool; //0non traité, 1 traité.
};
typedef struct arret2 arret2;


//Prend un arret2 et le nombre d'arret contenue dans le tableau d'arrets et retourne une liste chainée avec les arrets2 convenablement innitialisé.
//c'est a dire, arrets2 precedents, suivants, avec leurs pointeur vers arrets, arret2 papa, la distance totale de cet arret et arret_bool pour savoir si
//l'arret a été traité ou non.
void innit_arret2(arret2 *encre_arret2, int nb_arret){
    int i = 0; //on traite le premier arret du tableau

    arret2 *current = NULL; // on cree un arret2 pour manipulation
    current = malloc(sizeof(arret2));
    if (current==NULL){
        printf("Probleme sur malloc début innit arret2");
        exit(-5);
    }


    current->next = NULL;
    current->ptr_vers_arret = NULL;
    current->papa=NULL;

    //on traite le premier conteneur qui est lié précédemment par NULL
    current->distance_totale=-1;
    current->ptr_vers_arret=&tableau_d_arret[i];
    current->arret_bool=0;


    //on garde en mémoire le 1er conteneur
    encre_arret2=current;

    //un buffer pour chainer doublement.
    arret2 *arret2_buffer=NULL;

    //et on innit tout le conteneur avec les arrets, avec un temps à l'infini. (-1 élement deja traité)(-1 élément car on commence a 0)
    while(i < nb_arret-2){
        i++;
        //chainage double;
        current=current->next;
        current->next = NULL;
        current = malloc(sizeof(arret2));
        if (current==NULL){
            printf("Probleme sur malloc début innit arret2");
            exit(-5);
        }
        current->distance_totale=-1;
        current->ptr_vers_arret=&tableau_d_arret[i];
        current->arret_bool=0;
        current->papa=NULL;
    }

}

void launch_parcour_dijkstra(long depart, long arrivee, int nombre_arret){
    str_arrets *departi = NULL;
    str_arrets *arriveei = NULL;

    departi = arret_from_id(depart, nombre_arret);
    arriveei = arret_from_id(arrivee, nombre_arret);

    parcour_dijkstra(departi, arriveei);
}

int parcour_dijkstra(long *depart, long *arrivee, int nombre_arret){
    arret2 *encre_arret2 = NULL;


    //on a une liste - tableau - truc, contenant une liste de structure, ayant l'arret, le temps total, le truc (arret) precedent
    // et un bool pour savoir si on l'a traité ou non (a moins de faire une liste doublement chainé)
    innit_arret2(encre_arret2, nombre_arret);

    //un pointeur pour la manipulation
    arret2 *current = NULL;
    current = encre_arret2;
    //on cherche le départ
    fnc_traiter_depart(depart, current);
     //cherche et envoi l'élément le plus petit
    fnc_find_min(encre_arret2, current);
    //tant que current->arret->id != arrive
    while(current->ptr_vers_arret->stop_id != arrivee){
        //traité chacun des "fils" && "sort current && traite le current pour qu'il soit sur le chemin le plus court
        fnc_action_on son(current);
    }

    return 0;
}

//cherche l'arret 2 avec la distance_totale la plus courte
 void fnc_find_min(arret2 *premier_arret2, arret2 *arret2_return){
    arret2 * current = NULL;
    arret2 *current;
    int distance_trouve_min = DISTANCE_MAX_POSSIBLE;
    current = premier_arret2;
    while(current != NULL){
        //si distance totale n'est pas infini et le sommet n'a pas été traité
        // et la distance trouvé est inferieur à la distance min;
        if (current->distance_totale >= 0 && current->arret_bool==0 && current->distance_totale<distance_trouve_min){
            arret2_return=current;
            distance_trouve_min = current->distance_totale;
        }
    }
 }

void fnc_traiter_depart(long depart, arret2 *current){

    arret2 *buffer_prev = NULL;
    buffer_prev = current;

    current =  current->next;

    /*test lié au décalage de buffer*/
    if(buffer_prev->ptr_vers_arret->stop_id == depard){
        return buffer_prev;
    }
    else if(current->ptr_vers_arret->stop_id == depard){
        return current;
    }

    /*Parcours de la liste chainée*/
    while(depart!= current->ptr_vers_arret->stop_id ){
        /*accremente les 2 pointeurs*/
        buffer_prev = buffer_prev->next;
        current = current->next;
    }
    /*On pop le current*/
    buffer_prev->next = current->next;

    current->distance_totale=0;

    return current;
 }
