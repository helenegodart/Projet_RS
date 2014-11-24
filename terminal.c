#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "niveau.h"
#include "exit.h"
#include "paul.h"
#include "utilitaire.h"
#include "terminal.h"

int main(int argc, char *argv[])
{
	int continuer = 1;

	Commande *commande = malloc(sizeof(commande));
	initialiseCommande(commande);

	Niveau *niveau = malloc(sizeof(niveau));
	decompression(choixNiveau(argc, argv), commande, niveau);

	while(continuer){
		char *saisie = malloc(TAILLE_MAX_COMMANDE*sizeof(char));
		debutLigne(commande, niveau);
		fgets(saisie, TAILLE_MAX_COMMANDE, stdin);
		clean(saisie);
		strcpy(commande->commande, saisie);
		execution(commande, niveau);
		}
	return 0;
}

void execution(Commande *commande, Niveau *niveau){
	// fixDirectory(commande, niveau);
	int nbArgument = nbArg(commande);
	int ok = 1;
	int redirect = 1;
	// printf("nb arg : %d\n", nbArgument);
	exceptionProcessing(commande);
	// Vérifie si la commande est autorisée dans ce niveau
	if (!appartient(premierArg(commande->commande), niveau->charAutorise))
	{
		printf("Commande \"%s\" interdite !!\n", premierArg(commande->commande));
		printf("Liste des commandes autorisées :\n");
		String *temp = niveau->charAutorise->premier;
		printf("\t %s\n", temp->string);
		
		while((temp = temp->suivant) != NULL){
			printf("\t %s\n", temp->string);
		}
	}
	else
	{
		ListeString *listeArg = malloc(sizeof(ListeString));
		buildArgsChain(listeArg, commande);

		// Gestion particulière du cd
		if (!strcmp(substr(commande->commande,0,2), "cd"))
		{
			// Vérifie qu'il n'y a qu'un seul argument après le cd
			if (nbArgument == 1)
			{
				// Vérifie que le dossier visé existe et qu'il n'est pas un fichier et qu'il n'est pas au-dessus de la racine
				if (dirExists(listeArg->premier->string) && accessible(commande))
				{
					// Vérifie que ce n'est pas un fichier 
					if(fileExists(listeArg->premier->string))
						printf("\"%s\" est un fichier !!\n", listeArg->premier->string);
					else{
						chdir(listeArg->premier->string);
						fixDirectory(commande, niveau);
						commande->niveau = commande->niveau + incrementNiveau(commande);
					}
				}else
					printf("Le répertoire \"%s\" n'existe pas !!\n", listeArg->premier->string);
			}else{
				goBackRoot(commande);
			}
		}
		// gestion du pwd
		else if (!strcmp(substr(commande->commande,0,3), "pwd"))
			pwd(commande);
		// Gestion de >>
		else if((redirect = isRedirector(commande)) > 0){
			redirection(niveau, commande, redirect);
		}
		// commandes autres
		else
		{
			printf("%s", exec(listeArg, commande, niveau));
		}
	}
}

void debutLigne(Commande *commande, Niveau *niveau){
	fixDirectory(commande, niveau);
	fprintf(stdout,"%s > ", commande->directory);
}

void descriptifNiveau(Niveau *niveau){
	printf("DESCRIPTIF fichier meta\n");
	String *temp = niveau->charAutorise->premier;
	printf("charAutorise :\n\t %s\n", temp->string);
	
	while((temp = temp->suivant) != NULL){
		printf("\t %s\n", temp->string);
	}

	printf("Solution : %s\n", niveau->phraseMystere);
}

void initialiseCommande(Commande *c){
	c->commande = malloc(sizeof(char));
	c->directory = malloc(sizeof(char));
}

void determinationArgs(ListeString *liste, Commande *commande){
	char *temp = malloc(sizeof(char)*strlen(commande->commande));
	char *arg = malloc(sizeof(char)*strlen(commande->commande));
	strcpy(temp, commande->commande);
	arg = strtok(temp, " ");
	while((arg = strtok(NULL, " ")) != NULL)
	{
		insertionString(*(&liste), arg); 
	}
}

void exceptionProcessing(Commande *commande){
	ifExit(commande);
}

char *exec(ListeString *listeArg, Commande *commande, Niveau *niveau){
	int nbArgument = nbArg(commande);
	int fd[2];
	pipe(fd);
	int status;
	pid_t pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "Erreur fork\n");
        exit(EXIT_FAILURE);
    }
    if(pid == 0)
    {
    	if (nbArgument == 0)
		{
			close(fd[0]);
		   	dup2(fd[1], 1);
		   	close(fd[1]);
		   	strcpy(commande->commande, deleteSpaces(commande->commande));
			execlp(commande->commande, commande->commande, NULL);
		}else{
	    	char **tab = malloc(sizeof(char)*TAILLE_MAX_COMMANDE*nbArgument);
	    	creeTabArgs(tab, listeArg, nbArgument);
	    	close(fd[0]);
		   	dup2(fd[1], 1);
		   	close(fd[1]);
		   	strcpy(tab[0], deleteSpaces(tab[0]));
	        execvp(tab[0], tab);
	        free(tab);
	    }
	    exit(0);
    }else
    {
        waitpid(pid, &status, WCONTINUED);
        if (WIFEXITED(status))
        {
        	char *sortie = malloc(sizeof(char)*TAILLE_MAX_COMMANDE*200);
        	read(fd[0], sortie, TAILLE_MAX_COMMANDE*200);
        	verification(sortie, niveau);
        	return sortie;
        }
        else
        	return NULL;
    }
}
