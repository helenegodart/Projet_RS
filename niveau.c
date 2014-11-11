#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "niveau.h"


void creerNiveau(char *path, Niveau *niveau)
{
    int premierPassage = 1;
    FILE* fichier = NULL;
    
    fichier = fopen(path, "r");
    char ligne[TAILLE_MAX_COMMANDE] = "";

    char *string;
    char *indicateur;

    if (fichier == NULL)
    {
        printf("Erreur ouverture fichier !\nFichier \"%s\" inexistant !\n", path);
        exit(EXIT_FAILURE);
    }else
    {
        fgets(ligne, TAILLE_MAX_COMMANDE, fichier);
        while(!(!strcmp(ligne, "--"))){
            indicateur = substr(ligne, 0, 1);
            string = substr(ligne, 2, strlen(ligne)-3);
                if (!strcmp(indicateur, "$"))
                {
                    if (premierPassage)
                    {
                        niveau->charAutorise = initialisationString(string);
                        premierPassage = 0;
                    }else
                    {
                    insertionString(niveau->charAutorise, string);
                    }
                }
                else if (!strcmp(indicateur, "#"))
                {
                    //Ne rien faire, c'est un commentaire
                }
                else if (!strcmp(indicateur, ">"))
                {
                	niveau->phraseMystere = string;
                }
                else
                	printf("Fichier corrompu !\n");
                fgets(ligne, TAILLE_MAX_COMMANDE, fichier);
        }
 
        fclose(fichier);
    }
}

ListeString *initialisationString(char *c){
	ListeString *liste = malloc(sizeof(*liste));
    String *string = malloc(sizeof(*string));

    if (liste == NULL || string == NULL)
    {
        exit(EXIT_FAILURE);
    }

    string->string = c;
    string->suivant = NULL;
    liste->premier = string;

    return liste;
}

void insertionString(ListeString *liste, char *string){
    /* Création du nouvel élément */
    String *nouveau = malloc(sizeof(*nouveau));

    if (liste == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }
    nouveau->string = string;

    /* Insertion de l'élément au début de la liste */
    // printf("liste->premier : %s\n", liste->premier->string);
    // printf("nouveau->suivant : %s\n", nouveau->suivant->string);
    nouveau->suivant = liste->premier;
    liste->premier = nouveau;
}

char *substr(char *src,int pos,int len) { 
  char *dest=NULL;                        
  if (len>0) {                          
    dest = calloc(len+1, 1);       
    if(NULL != dest)
        strncat(dest,src+pos,len);            
  }                                       
  return dest;                            
}