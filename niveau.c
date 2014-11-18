#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "niveau.h"
#include "utilitaire.h"


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
        indicateur = substr(ligne, 0, 1);
        while(!(!strcmp(indicateur, "-"))){
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
                else{
                	fprintf(stderr,"Fichier corrompu !\n"); 
                    exit(EXIT_FAILURE);
                }
                
                fgets(ligne, TAILLE_MAX_COMMANDE, fichier);
                indicateur = substr(ligne, 0, 1);
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

void decompression(char *nom, Commande *commande, Niveau *niveau){
    char *decompresse = malloc(sizeof(char)*(strlen(nom)+7));
    int status;
    sprintf(decompresse, "%s.tar.gz", nom);
    
    int pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "Erreur fork\n");
        exit(EXIT_FAILURE);
    }
    if(pid == 0)
    {
        execlp("tar", "tar", "-xf", decompresse, NULL);
        exit(0);
    }else
    {
        waitpid(pid, &status, WCONTINUED);
        if (WIFEXITED(status))
        {
            chdir(nom);
            strcat(commande->directory, nom);
            commande->niveau = 0;
            // execlp("ls", "ls", NULL);
            creerNiveau("meta", niveau);
            // descriptifNiveau(niveau);
            remove("meta");   
        }
    }
}