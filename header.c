#include "header.h"
#include "distance_log_lat.h"



/*FUNCTION*/
/*Largeur*/

/*Note
*La partie largeur a été un peu moins commenté que les autres
*/

/**Fonction pour lancer une recherche en largeur à partir de 2 id d'arrets
*
*/
void fnc_chercher_une_destination(long id_station_depard, long id_station_arrivee, int NOMBRE_D_ARRET, int *cpt_de_noeud){

    str_arrets *depard;
    str_arrets *arrive;

    int i =0;
    while( id_station_depard != tableau_d_arret[i].stop_id)
    {
        i++;
        if(i>NOMBRE_D_ARRET-2){
            printf("On a pas trouvé le départ\n");
        }
    }

    depard = &tableau_d_arret[i];
    i =0;

        while( id_station_arrivee != tableau_d_arret[i].stop_id)
    {
        i++;
        if(i>NOMBRE_D_ARRET-2){
            printf("On a pas trouvé l'arrivée\n");
        }
    }
    arrive = &tableau_d_arret[i];

    fnc_parcour_en_largeur(depard, arrive, cpt_de_noeud);
}


/** Note
* Notre premiere fonction, avec une utilisation et comprehention des pointeurs qui n'est pas forcement adéquat
* Ce travail a eu le mérite de vraiment nous apprendre une bonne fois pour toute ces points précis en C (en plus du reste)
*/
int fnc_parcour_en_largeur(str_arrets *depard, str_arrets *arrive, int *cpt_de_noeud)
{
   int cpt_nb_noeud_totaux=0;

    /**<  Innitialisation des pointeurs*/
    str_noeud *noeud_papa, *noeud_enfant, *debut_liste_papa, *debut_liste_fils;

    noeud_papa = NULL;
    debut_liste_papa = NULL;
    debut_liste_fils = NULL;
    noeud_enfant = NULL;


    noeud_papa = malloc(sizeof(str_noeud));
    debut_liste_papa = malloc(sizeof(str_noeud));
    debut_liste_fils = malloc(sizeof(str_noeud));
    noeud_enfant = malloc(sizeof(str_noeud));

    if(!noeud_papa || !debut_liste_papa || !debut_liste_fils || !noeud_enfant){
        printf("Probleme à l'innit des pointeurs\n");
        return -666;
    }

    /**< Innitialisation du premier noeud */
    debut_liste_fils->ptr_vers_arret = depard;
    debut_liste_fils->papa = NULL; //ligne d'arrivée de la remonté
    debut_liste_fils->next = NULL;

    int arret_trouve = 0;

    int cpt_papa=0;
    int cpt_enfant =0;
    int cpt_level =1;


    debut_liste_papa=NULL; //techniquement on part de NULL...
     //le fils de NULL est le premier noeud


    //boucle primaire //tantque le noeud actuel n'est pas l'arrive
    //on change de niveau
    while(arret_trouve == 0){
       // plop("boucle 1");
        cpt_papa = 0;//nouveau niveau veut dire une nouvelle liste de papa futur a parcourir
        noeud_papa =debut_liste_fils;
        //boucle des freres //tantque qu'un papa existes ou que le papa n'est pas l'arrivée
        //on change de papa

        while(arret_trouve == 0 &&  noeud_papa != NULL){
          //  plop("Boucle 2");

            cpt_enfant = 0;//nouveau papa veut dire nouvelle liste d'enfant

            //liste de station est innitialiser a -1 s'il n'y a pas de destination, donc i_station == -1, il n'y a pas de station
            if(noeud_papa->ptr_vers_arret->i_station> -1)
            {
                //boucle enfant //tantqu'il reste une station destination à traitée au noeud actuel.
                //on change d'enfant
                while(arret_trouve == 0 && cpt_enfant <= noeud_papa->ptr_vers_arret->i_station){


                    //printf("it enfant : %d  nb enfant %d\n",cpt_enfant,noeud_papa->ptr_vers_arret->i_station  );
                   noeud_enfant =  fnc_noeud_suivant(noeud_enfant, noeud_papa, cpt_enfant );
                    //printf("noeud enfant %d\n", noeud_enfant);


                  //si on a trouvé la sortie
                   if(noeud_enfant->ptr_vers_arret == arrive){//FOUND IT !!!!
                    printf("Arrive trouvé\n\n");

                    arret_trouve = 1; /*On break les boucles*/
                    cpt_level++;
                   }


                    if(cpt_papa==0 && cpt_enfant == 0){


                        debut_liste_fils=noeud_enfant;
                    }
                   cpt_enfant++;
                }//fin du while enfant
                cpt_papa++;
            }
            else{

                //on passe au papa suivant
                if(noeud_papa->next == NULL)
                {
                    printf("Pas de fils et pas de frere ?!? %ld \n", noeud_papa->ptr_vers_arret->stop_id);
                    return -60;
                }
            }


            if(arret_trouve == 0){
                noeud_papa = noeud_papa->next; //jusqu'a ce que noeud_papa soit NULL

            }
            cpt_nb_noeud_totaux+=cpt_enfant;
        }
        cpt_level++;

        noeud_papa=debut_liste_fils; //descente d'un étage
        if (arret_trouve==1){
             noeud_papa =noeud_enfant;
        }
    }

    printf("compteur de level %d\n",cpt_level);

    //fonction traité le chemin
    while(noeud_papa != NULL)
    {
        printf("papa %d %ld %s \n",noeud_papa ,noeud_papa->ptr_vers_arret->stop_id,noeud_papa->ptr_vers_arret->stop_name);

        noeud_papa = noeud_papa->papa;
    }

    (*cpt_de_noeud) = cpt_nb_noeud_totaux;

    return 0;
}



/**On donne le pere et le numero de l'enfant a traiter
*on cree une noeud_enfant suivant //lié par le fait qu'il soit suivant
*on "remplit" ce nouveau noeud enfant
* puis on se "place" sur ce noeud enfant
*/
str_noeud * fnc_noeud_suivant(str_noeud * noeud_enfant, str_noeud * noeud_papa, int cpt_enfant){
    //suivant -> null

//    printf("Enfant au debut de la fonction %d ",noeud_enfant);
    //preparation du noeud enfant suivant;
    noeud_enfant->next = malloc(sizeof(str_noeud));
    if( noeud_enfant->next ==NULL){
        printf("Probleme dans le fnc_parcour");
        exit(-1);
    }

    //on "remplit" ce noeud enfant
    noeud_enfant->next->papa=noeud_papa;
    noeud_enfant->next->ptr_vers_arret = noeud_papa->ptr_vers_arret->stations_suivantes[cpt_enfant].to_stop_id;

    //on fait un sot pour se positionner sur le nouveau noeudt
    noeud_enfant = noeud_enfant->next;
    //on mes à null le noeud suivant.
    noeud_enfant->next= NULL;

//printf("    et ensuite %d\n", noeud_enfant);
    return  noeud_enfant;
}



/*FUNCTION*/
/*Profondeur*/
/** Rempli le tableau de str_arret avec des arrets
*
*/
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



/** Innitialise le tableau de str_arret
* en mettant toutes les id stations à -1 (non traité)
* et le nombre de stations à -1 (==0 station)
*/
void fnc_innit_tableau()
{
    for (int i =0; i<TAILLE_TAB; i++)
    {
        tableau_d_arret[i].stop_id=(-1);
        tableau_d_arret[i].i_station=(-1);
    }
}


/** Cree les destinations de chaque str_arret
*/
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


/** Gere les station double
* Cherche dans le tableau si deux stations (str_arret) on le même nom
* Regarde une a une les éléments du tableau de str_arret (sauf si c'est le même éléments : i==j)
* Si le nom est le même cree une nouvelle destination de str_arret A vers str_arret B
* On parcoure tous les éléments du tableau donc on finira par faire de B vers A
*/
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


/** Parcours en profondeur
*\param depart est un pointeur de str_arret qui represente l'arret de depart
*\param arrive est un pointeur de str_arret qui represente l'arret d'arrivee
*\param result
*\param profondeur
*\param cpt_noeud compteur du nombre de noeud que l'on a parcouru
*\param max_profondeur definie la profondeur maximun que l'on souhaite parcourir
* si arrivée trouvé renvoie 1 et affiche arret en cour, de facon recursive == affiche le chemin
*/
int parcours_profondeur(str_arrets *depart , str_arrets *arrive, int result, int profondeur, int *cpt_noeud, int max_profondeur){


    /*Déclaration working data*/
    int compteur = 0;

    /*Incrémentation Profondeur*/
    profondeur++;

    /*Test resultat trouvé*/
    if(depart == arrive){


        /*Affichage du resultat*/
        print_arret(depart);

        return 1;
    }



    /*Boucle de parcours des fils*/
    while(/*Test de profondeur*/profondeur < max_profondeur &&/*Test bool*/result == 0 &&/*Test enfant*/depart->i_station >= compteur){

            //on parcoure un nouveau noeud

            (*cpt_noeud)++;
            the_compteur++;

        /*Appel récurssif*/
        result = parcours_profondeur(depart->stations_suivantes[compteur].to_stop_id, arrive, result, profondeur, cpt_noeud, max_profondeur);
        compteur++;

         if (result == 1)
        {

            print_arret(depart);
            return result;
        }
    }

    return result;
}

/** Va lancer le parcour en profondeur avec des str_arrets comme depart et arrivée au lieu de leurs id en long)
*/
int launch_p_prof(int depart, int arrivee, int nombre_arret, int profondeur_max, int *cpt_noeud){

    /*Declaration working data*/

    int profondeur = 0;
    str_arrets *departi = NULL;
    str_arrets *arriveei = NULL;


    /*Init struct arret from ID*/
    departi = arret_from_id(depart, nombre_arret);
    arriveei = arret_from_id(arrivee, nombre_arret);


    return parcours_profondeur(departi, arriveei, 0, profondeur, cpt_noeud, profondeur_max);

}

/** Renvoi un pointeur de str_arret à partir d'un id d'arret
*/
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


/** Crée un nouveau noeud et le lie à son père
*/
str_noeud *link_dad_son(str_noeud *dad, int cpt, int *cpt_noeud){

    //on a crée un nouveau noeud
    (*cpt_noeud)++;
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

/**Augmente la limite de profondeur de x a chaque fois
* VALEUR_D_AUGMENTATION == 1
*/
void parcours_profondeur_borne(int depard, int arrivee, int nombre_arret, int profondeur_max, int *cpt_noeud){

    while(launch_p_prof(depard, arrivee, nombre_arret, profondeur_max, cpt_noeud)==0){
        printf("La profondeur max de %d n'a pas suffit elle augmente de + %d\n", profondeur_max, VALEUR_D_AUGMENTATION);
        printf("Nombres de noeuds parcouru %d\n", *cpt_noeud);
        profondeur_max+=VALEUR_D_AUGMENTATION;
    }

}






void print_arret(str_arrets * arret){
    printf("%ld %s %s \n%d\n", arret->stop_id, arret->stop_name, arret->stop_desc,
            arret->i_station);
}


/*FUNCTION*/
/*Dijkstra*/


/** Innitialisation des structures permettants d'utilisé Dijkstra et glouton
*Arguments
* [arret2 *ancre] == le debut de ma liste d'arret2
* [int nb_arret] == nombre d'arret contenue dans mon tableau de str_arret
*
* Pour chaque arret, on cree une structure arret2 avec :
* l'arret2 suivant (next)
* son papa initialiser à NULL
* un pointeur vers  un element deu tableau de str_arret (ptr_vers_arret)
* un boolean (int) pour savoir si l'arret a ete traite initialise a 0 (0 non traite / 1 traite)
* une distance totale innitialiser a l'infini (ici -1 represente l'infini)
*
*/
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
        /*Si premier element*/
        if (i==0){
            ancre->next = malloc(sizeof(arret2));

            if (ancre->next==NULL){
                printf("Probleme sur malloc début innit arret2");
                exit(-5);
            }
            current = ancre->next;
        }
        /*si element different du premier*/
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
        current->distance_totale=-1; /*initialisation dist : ~ à l'infini*/
        current->ptr_vers_arret=&tableau_d_arret[i];/*reference à la structure str_arret*/
        current->arret_bool=0;
        current->papa=NULL;
    }
}

/** Le parcour par Dijkstra
*
*/
int parcour_dijkstra(long depart, long arrivee, int nombre_arret, int *cpt_de_noeud){

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
       current = find_and_parse_neighborhood(smallest, ancre, arrivee, cpt_de_noeud);
       if(current == NULL){
        printf("current est à NULL\n");
       }

		//si on a trouver un sommet du resultat (plus petit)
		if (current != NULL && current->ptr_vers_arret->stop_id != arrivee){

            /*trouve le plus petit*/ /*fonction find_the_smallest_one*/
            smallest = NULL;
            smallest = find_the_smallest_one(ancre, cpt_de_noeud);
        }
	}

//	/*on traite l'arrivée*/
//	smallest->papa = current;
//	current = smallest;


    //on a une liste - tableau - truc, contenant une liste de structure, ayant l'arret, le temps total, le truc (arret) precedent
    // et un bool pour savoir si on l'a traité ou non (a moins de faire une liste doublement chainé)
    int distance_totale=0;
	while(current!=NULL){
		printf("%ld, %s %s\n",current->ptr_vers_arret->stop_id, current->ptr_vers_arret->stop_name, current->ptr_vers_arret->stop_desc);
		distance_totale += current->distance_totale;
		current=current->papa;
	}
	printf("\n\n\nVoici la duree totale du parcour  %d\n", distance_totale);

	//print_temps(ancre);
    return 0;
}




/** Permet de savoir si tous les sommets on été parcouru
* car nous ne retirons pas le sommets de S, mais on declare un boolean à 1
* non utilisé, car des que la distance minimun est -1, cela veut dire qu'il ne reste plus de sommets
* parcourable
*/
/*A t'on parcouru tous les sommets ? 0 si oui 1 sinon*/
int parcour_done( arret2 *ancre){

    /*Working data*/
    arret2 *current= NULL;
    current = ancre;

    /*Boucle de parcour de la liste des arrets*/
    while(current!=NULL){

        /*Si current arret = jamais selectionné*/
         if(current->arret_bool == 0){
            /*On break*/
            return 1;
         }

         /*Incrémentation*/
        current = current->next;

    }
    return 0;
}


/** A partir du début de la liste de arret2 (ancre), va parcourir toute la liste
* jusqu'a trouver le sommet qui possede la distance la plus petite
* ici la distance est representée par la durée du trajet
*
* on incremente le compteur de noeud de 1 à chaque fois que l'on parcour un nouveau noeud
*/
 arret2* find_the_smallest_one( arret2 *ancre, int *cpt_de_noeud){

    /*Wotrking data*/
    int min = DISTANCE_MAX;
    arret2 * current = NULL;
    arret2 * res = NULL;
    current = ancre;

    /*boucle de parcour de la liste*/
    int i=0;
    while(current->next!=NULL){
    i++;
        //on a vue un noeud
        if (current->arret_bool==0){
           (*cpt_de_noeud)= (*cpt_de_noeud)+1;
        }


         /*Si current arret n'a jamais ete selectionné, et que sa distance < au current min*/
         if(current->arret_bool == 0 && current->distance_totale != -1 && current->distance_totale <= min ){
            /*res pointe sur le nouveau candidat + mis a jour de la nouvelle val de min*/
            res = current;
            min = res->distance_totale;
         }
        current = current->next;
    }
    return res;
 }


/** Trouve les voisins du sommet le plus petit de S
* Modifie la valeur de distance des sommets
* Lie les sommets à leurs papa
* Retourne l'arrivée si elle fait partie des voisins
* -> on ne devrait pas faire cela, il est trop tard pour le changer
* je ne prefere pas modifier cela le jour du dépot
*/
arret2* find_and_parse_neighborhood(arret2 *current,  arret2 *ancre, long arrivee, int *cpt_de_noeud){

    /*Working data*/
    int i = 0;
    arret2 *walker = NULL;
    str_arrets *buffer = NULL;


    /* Tant qu'il reste un voisin*/
    while(i<=current->ptr_vers_arret->i_station){

        /*Nous récupérons et on stocke le nouveau voisin*/
        buffer = current->ptr_vers_arret->stations_suivantes[i].to_stop_id;
        walker = ancre;

        /*Parcour de la liste des arret2*/
        while( walker->next != NULL  && walker->ptr_vers_arret != buffer  ){
                walker = walker->next;

            //permet calculer la vrai complexité (on est censé retirer les sommets traité)
            if(walker->arret_bool==0){
                //On a vu un noeud
           (*cpt_de_noeud)= (*cpt_de_noeud)+1;
            }

        }

        /*Si l'arret pointé par le walker n'a jamais été séléctionné alors...*/
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






/*FUNCTION*/
/*Glouton*/

/*Parcour en Glouton*/
int parcour_glouton(long depart, long arrivee, int nombre_arret, int *cpt_de_noeud){

    /*Working data*/
    arret2 *ancre = NULL; //conteneur de tous les sommets
	arret2 *current=NULL; //le sommet que l'on manipule
	arret2 *buffer=NULL;

    //on crée notre conteneur
	ancre = malloc(sizeof(arret2));

	/*Gestion erreur malloc*/
	if (ancre==NULL){
		printf("Probleme sur malloc début innit arret2");
		exit(-5);
	}

	/*Init liste arret2*/
	/*on recup un pointeur sur l'adresse du départ, pas forcement le premier element*/
	current = innit_glouton(ancre, depart, arrivee, nombre_arret);

	/*Gestion erreur*/
	if(ancre == NULL){
        printf("Probleme d'innitialisation de l'ancre");
        return -23;
    }

    /*Boucle prinpale de traitement*/
    while(current->ptr_vers_arret->stop_id != arrivee){

        /*On stock le voisin le plus proche de l'arrivée dans le buffer*/
        buffer = find_neighborhood_glouton(current, ancre, arrivee, nombre_arret, cpt_de_noeud);

        /*Liaison au papa*/
        buffer->papa=current;

        /*Incrémentation*/
        current = buffer;

        /*Pblm affichage de la longeur totale... lié au ld "pblm lié à windows"*/
   //     printf("Arret : %s distance : %f\n",current->ptr_vers_arret->stop_name, current->distance_totale);

    }

        while(current != NULL){
        printf("Arret : %s distance : %f\n",current->ptr_vers_arret->stop_name, current->distance_totale);
        current = current->papa;
    }

return 0;
}

/*Innitialise Glouton en calculant la distance entre le sommet et l'arrivee*/
/*renvoi un pointeur sur l'arret "depart" correspondant au 2 eme arg*/
arret2 *innit_glouton(arret2 *ancre, long depart, long arrivee, int nombre_arret){

    /*Working data*/
    int i_nb_arret = 0;
    arret2 *current = NULL;
    /*buffer de resultat*/
    arret2 *res = NULL;

    /*recherche l'arrivée*/
    while(i_nb_arret < nombre_arret && tableau_d_arret[i_nb_arret].stop_id != arrivee){
        i_nb_arret++;
    }

    /*Initialisation de tt les sommets avec distance8max =-1*/
    innit_arret2(ancre, nombre_arret);
    current = ancre;

    /*Boucle d'initialisation des distances*/
    while(current !=NULL){
            /*On récupère le dépard afin de le renvoyer en sortie*/
            if(current->ptr_vers_arret->stop_id == depart){
                res = current;
            }

        /*Init distance*/
        current->distance_totale = fnc_distance_entre_point(current->ptr_vers_arret, &tableau_d_arret[i_nb_arret]);

        current = current->next;
    }


    return res;
}


/*calcule la distance entre 2 station (en metre)*/
double fnc_distance_entre_point(str_arrets *arret_a, str_arrets *arret_b){

    double distance_m=0;

    distance_m = distance(arret_a->stop_lat, arret_a->stop_lon, arret_b->stop_lat, arret_b->stop_lon, 'K');


    return (distance_m*1000);

}


arret2 *find_neighborhood_glouton(arret2 *current,arret2 *ancre, long arrivee, int nombre_arret, int *cpt_de_noeud){

    /*init working data*/
    int max = 99999999;
    int i = 0;
    arret2 *res = NULL;
    str_arrets *buffer = NULL;
    arret2 *buffer2 = NULL;
    arret2 *marrivee = NULL;
    marrivee = double_to_arret2(arrivee, ancre, nombre_arret);

    /*test de réussite*/
    if(marrivee == NULL){
        fprintf(stderr,"error to find arrivee in function find_neighborhood_glouton\n");
        exit(-1);
    }

    /*boucle de parcour de tt les voisin*/
    while(i   <=   current->ptr_vers_arret->i_station){


        /*buffer prend le valeur de ieme voisin*/
        buffer = current->ptr_vers_arret->stations_suivantes[i].to_stop_id;
        /*buffer2 prend la valeur de l'ancre(debut), pr parcour entier à chaque itération*/
        buffer2 = ancre;

        /*boucle de recherche equivalence buffer <-> buffer2*/
        while(buffer2 != NULL && buffer2->ptr_vers_arret != buffer){
            buffer2 = buffer2->next;

            //on a vue un noeud
            (*cpt_de_noeud)++;
        }

        /*cas particulier buffer = arrivee*/
        if(buffer2 == marrivee){
            return buffer2;
        }

        /*gestion erreur*/
        if(buffer2 == NULL){
            fprintf(stderr,"ERROR function find_neighborhood_glouton\n");
            exit(-1);
        }

        /*Comparaison avec la valeur max de reference*/
        if(buffer2->distance_totale < max){
            /*Si nouveau max, attribution de la valeur de retour + redefinition du max*/
            res = buffer2;
            max = res->distance_totale;
        }
        i++;
    }

    /*Gestion erreur*/
    if(res==NULL){
        printf("ERREUR SOLUTION NOT FOUND\n");
        exit(-1);
    }
    return res;
}

/*Fonction permettant la traduction d'un arret double en structure arret 2*/
arret2 *double_to_arret2(long in, arret2 *ancre, int nombre_arret){

    /*Working data*/
    arret2 *buffer = NULL;
    buffer = ancre;

    /*Boucle de recherche dans la liste de structure de type arret2 (a partir d'ancre)*/
    while(buffer!=NULL && buffer->ptr_vers_arret->stop_id != in){
        buffer = buffer->next;
    }

    /*Test de réussite*/
    if(buffer == NULL){
        printf("ERROR FUNCTION double_to_arret2\n");
        exit(-1);
    }

    return buffer;
}
