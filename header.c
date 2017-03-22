//
//  header.c
//  parcours_en_profondeur
//
//  Created by Pichot on 20/03/2017.
//  Copyright © 2017 Pichot. All rights reserved.
//

#include "header.h"



/*FUNCTION*/

int fnc_remplir_arret(FILE* arret)
{

    char buffer_ligne[TAILLE_TAB];
    char *buffer_element;

    //commencé a la 2eme ligne
    fgets(buffer_ligne, 4096, arret);


    int i=0;
    //tant qu'il reste une ligne
    while( fgets(buffer_ligne, 4096, arret) )
    {
        //long stop_id;
        buffer_element = strtok(buffer_ligne, ",");
        if (buffer_element == NULL)
        {
            printf("Probleme de strtok long stop_id\n");
            return -11;
        }
        //printf("stop id :%s  ", buffer_element);

        char*ptr;
        tableau_d_arret[i].stop_id = strtol(buffer_element, &ptr, 10);

        //stop_code n'est pas a faire strtok passe tous les elements non utilse

        //char stop_name[1024];
        buffer_element = strtok(NULL, ",");
        if (buffer_element == NULL)
        {
            printf("Probleme de strtok char stop_name[1024]\n");
            return -2;
        }
        strcpy(tableau_d_arret[i].stop_name, buffer_element);
        //printf("stop_name :%s  ", buffer_element);
        //char stop_desc[1024];
        buffer_element = strtok(NULL, ",");
        if (buffer_element == NULL)
        {
            printf("Probleme de strtok char stop_desc[1024]\n");
            return -3;
        }
        strcpy(tableau_d_arret[i].stop_desc, buffer_element);
        //printf("stop_desc :%s  ", buffer_element);
        //double stop_lat;
        buffer_element = strtok(NULL, ",");
        if (buffer_element == NULL)
        {
            printf("Probleme de strtok char double stop_lat\n");
            return -4;
        }
        tableau_d_arret[i].stop_lat = atof(buffer_element);
        //double stop_long;
        buffer_element = strtok(NULL, ",");
        if (buffer_element == NULL)
        {
            printf("Probleme de strtok double stop_long\n");
            return -5;
        }
        tableau_d_arret[i].stop_lon = atof(buffer_element);
        //printf("stop_lat :%s \n ", buffer_element);
        ////        int location_type;
        //        strtok(buffer_element, ",");
        ////        pas utilisé
        //
        ////        int parent_station;
        //        strtok(buffer_element, ",");
        ////        pas utilisé

        tableau_d_arret[i].stations_suivantes = malloc(sizeof(str_arcs_transfert)*20);
        for (int p = 0; p<TAILLE_DESTINATION-1; p++)
        {

            tableau_d_arret[i].stations_suivantes[p].to_stop_id =NULL;
        }
        i++;
    }
    return 0;
}




void fnc_innit_tableau()
{
    for (int i =0; i<TAILLE_TAB; i++)
    {
        tableau_d_arret[i].stop_id=(-1);
        tableau_d_arret[i].i_station=(-1);
    }
}



int fnc_remplir_trajet(FILE* arcs, FILE* transferts, int nombre_d_arret)
{

    char buffer_ligne[TAILLE_TAB];
    char *buffer_element;
//    arcs
//    commencé a la 2eme ligne
    fgets(buffer_ligne, 4096, arcs);
    int i_arc_transfert=0;
//    tant qu'il reste une ligne
    while( fgets(buffer_ligne, 4096, arcs) )
    {
        i_arc_transfert++; //permet de savoir qu'elle ligne on lit
//      int from_stop_id;

        buffer_element = strtok(buffer_ligne, ",");
        if (buffer_element == NULL)
        {
            printf("Probleme de strtok from_stop_id arc\n");
            return -1;
        }


//      on cherche from_stop dans struct_arrets
        char*ptr; //utilisé par strtol (string to long)
        int j = 0 ;
        long from_to_stop_id =0;
        from_to_stop_id = strtol(buffer_element, &ptr, 10);
        while( (tableau_d_arret[j].stop_id!= from_to_stop_id  ) && j<nombre_d_arret-1)
        {
            j++;
        }
        //j est l'arret avec l'id  == buffer_element == départ
//      on s'assure que l'on a bien trouvé un arret compatible
        if (tableau_d_arret[j].stop_id== from_to_stop_id)
        {
    //      on fait la même chose avec to_stop_id
            from_to_stop_id =0;
            buffer_element = strtok(NULL, ",");
            if (buffer_element == NULL)
            {
                printf("Probleme de strtok to_stop_id arc\n");
                return -1;
            }
            from_to_stop_id = strtol(buffer_element, &ptr, 10);
            int k = 0 ;
            while( (tableau_d_arret[k].stop_id!= from_to_stop_id ) && j<nombre_d_arret)
            {
                k++;
            }
            //k est l'arret avec l'id  == buffer_element == arrivé
            if (tableau_d_arret[k].stop_id== from_to_stop_id )
            {
                   //      on ajoute la destination à l'arret
    //      on oublie pas d'indiqué que le nombre de destination de cet arrets à augmenté de 1;
            tableau_d_arret[j].i_station++;
            tableau_d_arret[j].stations_suivantes[tableau_d_arret[j].i_station].to_stop_id = &tableau_d_arret[k];
    //      et les éléments de la destination
    //      link_time
            buffer_element = strtok(NULL, ",");

            if (buffer_element == NULL)
            {
                printf("Probleme de strtok to_stop_id link_time\n");
                return -1;
            }

            tableau_d_arret[j].stations_suivantes[tableau_d_arret[j].i_station].temps_trajet = atoi(buffer_element);

            //      les arcs n'ont pas de transfer_type, donc on donne la valeur -1 pour identifier un arc_tranferts en t'en qu'arc
            tableau_d_arret[j].stations_suivantes[tableau_d_arret[j].i_station].transfer_type=(-1);
            }else
            {
                printf("L'arcs n°:%d n'a pas trouvé d'arrets d'arrivé", i_arc_transfert);
            }

        }else
        {
            printf("L'arcs n°:%d n'a pas trouvé d'arrets de départ", i_arc_transfert);
        }

    }

//    transferts
//    commencé a la 2eme ligne
    fgets(buffer_ligne, 4096, transferts);
    i_arc_transfert=0;
//    tant qu'il reste une ligne
    while( fgets(buffer_ligne, 4096, transferts) )
    {
        i_arc_transfert++;
//      int from_stop_id;

        buffer_element = strtok(buffer_ligne, ",");
        if (buffer_element == NULL)
        {
            printf("Probleme de strtok from_stop_id arc\n");
            return -1;
        }


//      on cherche from_stop dans struct_arrets
        char*ptr; //utilisé par strtol (string to long)
        int j = 0 ;
        long from_to_stop_id =0;
        from_to_stop_id = strtol(buffer_element, &ptr, 10);

        while( (tableau_d_arret[j].stop_id!= from_to_stop_id  ) && j<nombre_d_arret-1)
        {
            j++;
        }

//      on s'assure que l'arret soit dans la liste sinon on passe à l'arc suivant
        if (tableau_d_arret[j].stop_id== from_to_stop_id )
        {
    //      on fait la même chose avec to_stop_id
            buffer_element = strtok(NULL, ",");
            from_to_stop_id =0;
            if (buffer_element == NULL)
            {
                printf("Probleme de strtok to_stop_id arc\n");
                return -1;
            }
            from_to_stop_id = strtol(buffer_element, &ptr, 10);
            int k = 0 ;
            while( (tableau_d_arret[k].stop_id!= from_to_stop_id ) && j<nombre_d_arret)
            {
                k++;
            }
            if (tableau_d_arret[k].stop_id== from_to_stop_id)
            {
    //      on ajoute la destination à l'arret
    //      on oublie pas d'indiqué que le nombre de destination de cet arrets à augmenté de 1;
            tableau_d_arret[j].i_station++;
            tableau_d_arret[j].stations_suivantes[tableau_d_arret[j].i_station].to_stop_id = &tableau_d_arret[k];
    //      et les éléments de la destination
    //      les arcs n'ont pas de transfer_type, donc on donne la valeur -1 pour identifier un arc_tranferts en t'en qu'arc
            buffer_element = strtok(NULL, ",");
            if (buffer_element == NULL)
            {
                printf("Probleme de strtok to_stop_id transfer_type\n");
                return -1;
            }
            tableau_d_arret[j].stations_suivantes[tableau_d_arret[j].i_station].transfer_type=atoi(buffer_element);
    //      link_time
            buffer_element = strtok(NULL, ",");
            if (buffer_element == NULL)
            {
                printf("Probleme de strtok to_stop_id link_time\n");
                return -1;
            }
            tableau_d_arret[j].stations_suivantes[tableau_d_arret[j].i_station].temps_trajet = atoi(buffer_element);
            }else
            {
                printf("Le trajet n°:%d n'a pas trouvé d'arrets d'arrivé", i_arc_transfert);
            }

        }else
        {
            printf("Le trajet n°:%d n'a pas trouvé d'arrets de départ", i_arc_transfert);
        }
    }



return 0;
}



void fnc_station_vers_leurs_double(int nombre_d_arret)
{
    int i = 0;
    int j = 0;
    //on teste toute la liste
    while(i<nombre_d_arret-1)
    {
        j=0;
        while(j<nombre_d_arret-1)
        {
            //on ne teste pas l'arret avec lui même
            if (i!=j)
            {


                //si les noms sont les meme alors ils ont peut voyager
                if(strcmp(tableau_d_arret[i].stop_desc, tableau_d_arret[j].stop_desc)==0 )
                {
                    tableau_d_arret[i].i_station++;
                    tableau_d_arret[i].stations_suivantes[tableau_d_arret[i].i_station].temps_trajet=60;
                    tableau_d_arret[i].stations_suivantes[tableau_d_arret[i].i_station].transfer_type = -2;
                    tableau_d_arret[i].stations_suivantes[tableau_d_arret[i].i_station].to_stop_id = &tableau_d_arret[j];

                }
            }
            j++;
        }
        i++;
    }
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
    return parcours_profondeur(departi, arriveei, 0, profondeur, cpt_noeud, profondeur_max);

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



/*Dijkstra*/

//Prend un arret2 et le nombre d'arret contenue dans le tableau d'arrets et retourne une liste chainée avec les arrets2 convenablement innitialisé.
//c'est a dire, arrets2 precedents, suivants, avec leurs pointeur vers arrets, arret2 papa, la distance totale de cet arret et arret_bool pour savoir si
//l'arret a été traité ou non.
//void innit_arret2(arret2 *ancre, int nb_arret){
//
//    int i = 0; //on traite le premier arret du tableau
//
//
//    ancre->next = NULL;
//    ancre->ptr_vers_arret = NULL;
//    ancre->papa=NULL;
//
//    //on traite le premier conteneur qui est lié précédemment par NULL
//    ancre->distance_totale=-1;
//    ancre->ptr_vers_arret=&tableau_d_arret[i];
//    ancre->arret_bool=0;
//
//    //on garde en mémoire le 1er conteneur //ANCRE
//    arret2 *current = NULL; // on cree un arret2 pour manipulation
//    ancre->next = malloc(sizeof(arret2));
//    if (ancre->next==NULL){
//        printf("Probleme sur malloc début innit arret2");
//        exit(-5);
//    }
//    current = ancre->next;
//
//    //et on innit tout le conteneur avec les arrets, avec un temps à l'infini. (-1 élement deja traité)(-1 élément car on commence a 0)
//    while(i < nb_arret-2){
//
//        i++;
//        current->next = NULL;
//        current->distance_totale=-1;
//        current->ptr_vers_arret=&tableau_d_arret[i];
//        current->arret_bool=0;
//        current->papa=NULL;
//
//
//        current->next = malloc(sizeof(arret2));
//        if (current->next==NULL){
//            printf("Probleme sur malloc début innit arret2");
//            exit(-5);
//        }
//
//        current = current->next;
//
//    }
//
//}
void innit_arret2(arret2 *ancre, int nb_arret){

    int i = 0; //on traite le premier arret du tableau


    ancre->next = NULL;
    ancre->ptr_vers_arret = NULL;
    ancre->papa=NULL;

    //on traite le premier conteneur qui est lié précédemment par NULL
    ancre->distance_totale=-1;
    ancre->ptr_vers_arret=&tableau_d_arret[i];
    ancre->arret_bool=0;

    //on garde en mémoire le 1er conteneur //ANCRE
    arret2 *current = NULL; // on cree un arret2 pour manipulation


    //et on innit tout le conteneur avec les arrets, avec un temps à l'infini. (-1 élement deja traité)(-1 élément car on commence a 0)
    while(i < nb_arret-2){
        if (i==0){
            ancre->next = malloc(sizeof(arret2));
            if (ancre->next==NULL){
                printf("Probleme sur malloc début innit arret2");
                exit(-5);
            }
            current = ancre->next;
        }
        else
        {
           current->next = malloc(sizeof(arret2));
            if (current->next==NULL){
                printf("Probleme sur malloc début innit arret2");
                exit(-5);
            }
            current = current->next;
        }

        i++;
        current->next = NULL;
        current->distance_totale=-1;
        current->ptr_vers_arret=&tableau_d_arret[i];
        current->arret_bool=0;
        current->papa=NULL;
    }
}

//void launch_parcour_dijkstra(long depart, long arrivee, int nombre_arret){
//    str_arrets *departi = NULL;
//    str_arrets *arriveei = NULL;
//
//    departi = arret_from_id(depart, nombre_arret);
//    arriveei = arret_from_id(arrivee, nombre_arret);
//
//    parcour_dijkstra(departi, arriveei, nombre_arret);
//}

int parcour_dijkstra(long depart, long arrivee, int nombre_arret){

	arret2 *ancre = NULL; //conteneur de tous les sommets
	arret2 *current=NULL; //le sommet que l'on manipule

	//on crée notre conteneur
	ancre = malloc(sizeof(arret2));
	if (ancre==NULL){
		printf("Probleme sur malloc début innit arret2");
		exit(-5);
	}
	innit_arret2(ancre, nombre_arret);
	if(ancre == NULL){
        printf("Probleme d'innitialisation de l'ancre");
        return -23;
    }

	//on recupere le depart
	int i_pour_depart =0;
	current=ancre;
	while(current != NULL && current->ptr_vers_arret->stop_id != depart ){
		current = current->next;
		i_pour_depart++;
    }

	if(current == NULL){
        printf("Pas de depart trouvé\n");
        return -21;
    }

	// on innitialise la distance le depart
	current->distance_totale=0;


	//t'en qu'on trouve un sommet plus petit
//	int i_test_le_parcour = 0;
    arret2 *smallest = NULL;
    /*Le départ (current) étant le 1er plus petit (smallest)*/
    smallest = current;

	while(current!= NULL && current->ptr_vers_arret->stop_id != arrivee){
	//while(parcour_done(ancre) == 1){ //utilisable avec test si min =-1 arreter

		//on dit avoir traité le sommet
		smallest->arret_bool=1;
		/*Récupération, analyse, traitement de ses voisins*/
       current = find_and_parse_neighborhood(smallest, ancre, arrivee);
       if(current == NULL){
        printf("current est à NULL\n");
       }

		//si on a trouver un sommet du resultat (plus petit)
		if (current != NULL && current->ptr_vers_arret->stop_id != arrivee){

            /*trouve le plus petit*/ /*fonction find_the_smallest_one*/
            smallest = NULL;
            smallest = find_the_smallest_one(ancre);
        }
	}

//	/*on traite l'arrivée*/
//	smallest->papa = current;
//	current = smallest;


    //on a une liste - tableau - truc, contenant une liste de structure, ayant l'arret, le temps total, le truc (arret) precedent
    // et un bool pour savoir si on l'a traité ou non (a moins de faire une liste doublement chainé)
    int distance_totale=0;
	while(current!=NULL){
		printf("%ld, %s %s ---%d\n",current->ptr_vers_arret->stop_id, current->ptr_vers_arret->stop_name, current->ptr_vers_arret->stop_desc, current->distance_totale);
		distance_totale += current->distance_totale;
		current=current->papa;
	}
	printf("\n\n\n%d\n", distance_totale);

	//print_temps(ancre);
    return 0;
}



void bool_to_true(long id, arret2 *current){

    /*Parcours de la liste chainé*/
    while(current != NULL && current->ptr_vers_arret->stop_id != id ){/*tant que l'id du current != id (en argument)*/
        /*accremente le pointeur*/
        current = current->next;
    }
    current->arret_bool = 1; /*Convention : true corres à : sommet traité, cf structure*/
}

/*A t'on parcouru tous les sommets ? 0 si oui 1 sinon*/
int parcour_done(const arret2 *ancre){

    arret2 *current= NULL;
    current = ancre;

    while(current!=NULL){

         if(current->arret_bool == 0){
            return 1;
         }
        current = current->next;

    }
    return 0;
}

 arret2* find_the_smallest_one(const arret2 *ancre){
    arret2 * current = NULL;
    arret2 * res = NULL;
    current = ancre;
    //print_temps(ancre);

    int min = DISTANCE_MAX;

    while(current->next!=NULL){

         if(current->arret_bool == 0 && current->distance_totale != -1 && current->distance_totale <= min ){
            res = current;
            min = res->distance_totale;
         }
        current = current->next;
    }
    return res;
 }

arret2* find_and_parse_neighborhood(arret2 *current, const arret2 *ancre, long arrivee){

    int i = 0;
    arret2 *walker = NULL;
    str_arrets *buffer = NULL;
	arret2 *buffer_parcour_Dijkstra = NULL;

    /* Tant qu'il reste un voisin*/
    while(i<=current->ptr_vers_arret->i_station){
        buffer = current->ptr_vers_arret->stations_suivantes[i].to_stop_id;
        walker = ancre;
        /*Parcour de la liste des arret2*/
        while( walker->next != NULL  && walker->ptr_vers_arret != buffer  ){
                walker = walker->next;
        }

        if(walker->arret_bool==0){

            if(walker == NULL){
                printf("Walker == NULL\n");
                exit(-6);
            }

            /*Valorisation distance voisin*/
            //walker->distance_totale = buffer->stations_suivantes[i].temps_trajet + current->distance_totale ;

            /*traitement*/

            /*Cas particulier, nous avons trouvé l'arivée*/
            if(walker->ptr_vers_arret->stop_id == arrivee){
                walker->papa = current;
                current = walker;
                return current;
            }
            /*test de temps*/
            else if(walker->distance_totale == -1 || walker->distance_totale > (current->distance_totale  + current->ptr_vers_arret->stations_suivantes[i].temps_trajet)){
                /*valorisation du nouveau temps*/
                walker->distance_totale =(current->distance_totale  + current->ptr_vers_arret->stations_suivantes[i].temps_trajet);
               // printf("di->%d   dj->%d   val(ij)->%d\n", walker->distance_totale, current->distance_totale, current->ptr_vers_arret->stations_suivantes[i].temps_trajet );

                // /*valorisation pred voisin*/
                walker->papa = current;
               // print_arret2(walker);
            }
        }
        i++;
    }
    //print_temps(ancre);
	/*On "retourne" le sommet ou NULL*/
	return  walker;
}

void print_temps(const arret2 *ancre){
   arret2 *current = NULL;
   current = ancre;

    while(current != NULL){
            if(current->distance_totale>-1){
                 printf("Arret : %ld, distance : %d, traité : %d\n",current->ptr_vers_arret->stop_id, current->distance_totale, current->arret_bool);
            }

        current = current->next;
    }
}

void print_arret2(arret2 *current){

    printf("Arret : %ld, %s %s distance : %d, traité : %d\n",current->ptr_vers_arret->stop_id, current->ptr_vers_arret->stop_name, current->ptr_vers_arret->stop_desc,  current->distance_totale, current->arret_bool);
}





//
///*Augmente la limite de profondeur de x a chaque fois*/
//void parcours_profondeur_borne(int depard, int arrivee, int nombre_arret, int profondeur_max){
//
//    while(launch_p_prof(depard, arrivee, nombre_arret, profondeur_max)==0){
//      profondeur_max+=1;
//    }
//
//}


///*Parcour en Glouton*/
//int parcour_glouton(int depard, int arrivee ){
//
//    arret2 *ancre = NULL; //conteneur de tous les sommets
//	arret2 *current=NULL; //le sommet que l'on manipule
//	arret2 *le_debut_parcour = NULL; //le debut du resultat de glouton
//	arret2 *le_parcour = NULL; //"l'actuel" du resultat de glouton
//
//    //on crée notre conteneur
//	ancre = malloc(sizeof(arret2));
//	if (ancre==NULL){
//		printf("Probleme sur malloc début innit arret2");
//		exit(-5);
//	}
//	innit_glouoton(ancre, depart, nombre_arret);
//	if(ancre == NULL){
//        printf("Probleme d'innitialisation de l'ancre");
//        return -23;
//    }
//
//    //si trouvé
//    //afficher
//    //retourner 1
//    //sinon retourner 0
//
//}

////pour sommet calculer la distance entre le sommet et le depart
//void innit_glouton(arret2 *ancre, arret *depart, int nombre_arret){
//
//    while(ancre!= NULL){
//        ancre->distance_totale =fnc_distance_entre_point(ancre->ptr_vers_arret, depart);
//    }
//
//
//}


/*calcule la distance entre 2 station (en metre)*/
//int fnc_distance_entre_point(str_arrets *arret_a, str_arrets *arret_b){
//
//    double  = distance_m;
//    distance_m = distance(arret_a->stop_lat, arret_a->stop_lon, arret_b->stop_lat, arret_b->stop_lon, 'K');
//
//    return (distance_m*1000);
//
//}



//int a_etoile(){
//}



