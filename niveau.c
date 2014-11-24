#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "niveau.h"
#include "utilitaire.h"


void creerNiveau(char *path, Niveau *niveau, char *nomNiveau)
{
    int premierPassage = 1, continuer = 1;
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
        int i = 0;
        fgets(ligne, TAILLE_MAX_COMMANDE, fichier);
        indicateur = substr(ligne, 0, 1);
        while(continuer){
            if (!(!strcmp(ligne, "\n")))
            {
                string = substr(ligne, 2, strlen(ligne)-3);
                if (!strcmp(indicateur, "$"))
                {
                    if (premierPassage)
                    {
                        niveau->charAutorise = malloc(sizeof(niveau->charAutorise));
                        initialisationString(niveau->charAutorise, string);
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
                    continuer = 0;
                }
                else{
                	fprintf(stderr,"Fichier corrompu !\n"); 
                    exit(EXIT_FAILURE);
                }
            }
                
            fgets(ligne, TAILLE_MAX_COMMANDE, fichier);
            indicateur = substr(ligne, 0, 1);
        }
 
        fclose(fichier);
    }

    niveau->nom = malloc(TAILLE_MAX_COMMANDE*sizeof(char));
    strcpy(niveau->nom, nomNiveau);
}

void initialisationString(ListeString *liste, char *c){
    String *string = malloc(sizeof(*string));

    if (liste == NULL || string == NULL)
    {
        exit(EXIT_FAILURE);
    }

    string->string = c;
    string->suivant = NULL;
    liste->premier = string;
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
    char *nomNiveau = malloc(sizeof(char)*strlen(nom)+4);
    char *decompresse = malloc(sizeof(char)*(strlen(nom)+7));
    int status;
    sprintf(decompresse, "%s.tar.gz", nom);
    
    if (!fileExists(decompresse))
    {
        sprintf(decompresse, "%s.tgz", nom);
        if (!fileExists(decompresse))
        {
            printf("Ouverture \"%s\" impossible !!\n", decompresse);
        }
    }
    
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
            // chdir(nom);
            strcat(commande->directory, nom);
            commande->niveau = 0;
            // execlp("ls", "ls", NULL);
            strcpy(nomNiveau, nom);
            creerNiveau("meta", niveau, nomNiveau);
            // descriptifNiveau(niveau);
            remove("meta");   
        }
    }
    free(decompresse);
}