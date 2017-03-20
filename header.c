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

