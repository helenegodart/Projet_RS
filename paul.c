#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "niveau.h"
#include "terminal.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "utilitaire.h"
#include "paul.h"
#include <errno.h>


int isRedirector(Commande *commande){
	if(strstr(commande->commande, ">>") != NULL)
		return ENVOI_W;
	else if(strstr(commande->commande, ">") != NULL)
		return ENVOI_WR;
	else if(strstr(commande->commande, "<<") != NULL)
		return RECOI_W;
	else if(strstr(commande->commande, "<") != NULL)
		return RECOI_WR;
	else
		return -1;
}

void redirection(Commande *commande, int mode){	
	if (mode == ENVOI_W)
	{
		printf("ENVOI_W\n");
		writeFile(commande, ">>");
	}
	else if (mode == ENVOI_WR)
	{
		printf("ENVOI_WR\n");
		writeFile(commande, ">");
	}
}

void writeFile(Commande *commande, char *token){
	char *fichier = malloc(sizeof(char)*strlen(commande->commande));
	char *ligne = malloc(sizeof(char)*strlen(commande->commande));
	strcpy(ligne, commande->commande);
	char *temp = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
	char *com = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
	com = strtok(ligne, token);
	fichier = strtok(NULL, token);
	fichier = deleteSpaces(fichier);
	FILE *f = NULL;

	if(!strcmp(token, ">>"))
		f = fopen(fichier, "a");
	else if(!strcmp(token, ">"))
		f = fopen(fichier, "w");

	Commande *newCommande = malloc(sizeof(newCommande));
	initialiseCommande(newCommande);
	strcpy(newCommande->commande, com);
	strcpy(newCommande->directory, commande->directory);
	newCommande->niveau = commande->niveau;

	if (f == NULL)
	{
		printf("erreur ouverture %s\n", fichier);
		exit(EXIT_FAILURE);
	}else{
		ListeString *listeArg = malloc(sizeof(ListeString));
		buildArgsChain(listeArg, newCommande);
		char *result = exec(listeArg, newCommande);
		fputs(result, f);
		fclose(f);
	}
}

char *deleteSpaces(char *chain){
    char *newChain = malloc(strlen(chain)*sizeof(char)+1);
    int i, j = 0;

    for(i=0;i<strlen(chain)+1;i++){
    	if(chain[i] != ' ')
        {
           newChain[j++] = chain[i];
        }
    }
    return newChain;
}