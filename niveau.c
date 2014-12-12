#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>
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
        printw("Erreur ouverture fichier !\nFichier \"%s\" inexistant !\n", path);
        endwin();
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
        		else if (!strcmp(indicateur, "&"))
        		{
        			niveau->consigne = string;
        		}	
                else{
                	wprintw(stdscr,"Fichier corrompu !\n"); 
                    endwin();
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
    // fprintf(stderr, "niveau->nom : %s\n", niveau->nom);

    niveau->history = malloc(sizeof(niveau->history));
    initialisationString(niveau->history, "exit");
}

void initialisationString(ListeString *liste, char *c){
    String *string = malloc(sizeof(*string));

    if (liste == NULL || string == NULL)
    {
        endwin();
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
        endwin();
        exit(EXIT_FAILURE);
    }
    nouveau->string = malloc(sizeof(char)*strlen(string));
    strcpy(nouveau->string, string);

    /* Insertion de l'élément au début de la liste */
    // printw("liste->premier : %s\n", liste->premier->string);
    // printw("nouveau->suivant : %s\n", nouveau->suivant->string);
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
        free(decompresse);
        decompresse = malloc(sizeof(char)*(strlen(nom)+7));
        sprintf(decompresse, "%s.tgz", nom);
        if (!fileExists(decompresse))
        {
            printw("Ouverture \"%s\" impossible !!\n", decompresse);
        }
    }

    if(mkdir(nom, 0777) < 0){
        wprintw(stdscr, "Erreur de création du dossier \"%s\"\n", nom);
        endwin();
        exit(EXIT_FAILURE);
    }
    else
    {   char *dest = malloc(sizeof(char)*(strlen(decompresse)+strlen(nom)+5));
        sprintf(dest, "%s/%s", nom, decompresse);
        copier_fichier(decompresse, dest);
        chdir(nom);
        int pid = fork();
        if (pid == -1)
        {
            wprintw(stdscr, "Erreur fork\n");
            endwin();
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
                strcat(commande->directory, nom);
                commande->niveau = 0;
                // execlp("ls", "ls", NULL);
                strcpy(nomNiveau, nom);
                // fprintf(stderr, "nomNiveau : %s\n", nomNiveau);
                creerNiveau("meta", niveau, nomNiveau);
                // descriptifNiveau(niveau);
                remove("meta");
            }
        }
        remove(decompresse);
        free(decompresse);
        free(nomNiveau);
        free(dest);
    }
}

int copier_fichier(char *source, char *destination) 
{ 
    FILE* fSrc; 
    FILE* fDest; 
    char buffer[1024]; 
    int NbLus; 
  
    if ((fSrc = fopen(source, "rb")) == NULL) 
    { 
        return -1; 
    } 
  
    if ((fDest = fopen(destination, "wb")) == NULL) 
    { 
        fclose(fSrc); 
        return -2; 
    } 
  
    while ((NbLus = fread(buffer, 1, 1024, fSrc)) != 0) 
        fwrite(buffer, 1, NbLus, fDest); 

    fclose(fDest); 
    fclose(fSrc); 
  
    return 0; 
}
