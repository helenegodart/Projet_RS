#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "niveau.h"
#include "utilitaire.h"
#include "terminal.h"

int main(int argc, char const *argv[])
{
	int continuer = 1;

	Commande *commande = malloc(sizeof(commande));
	initialiseCommande(commande);

	Niveau niveau;
	// creerNiveau("niveau1", &niveau);
	// descriptifNiveau(&niveau);
	decompression("niveau1", commande, &niveau);

	while(continuer){
		char *saisie = malloc(TAILLE_MAX_COMMANDE*sizeof(char));
		debutLigne(commande);
		fgets(saisie, TAILLE_MAX_COMMANDE, stdin);
		clean(saisie);
		strcpy(commande->commande, saisie);
		execution(commande, &niveau);
		}
	return 0;
}

void execution(Commande *commande, Niveau *niveau){
	int nbArgument = nbArg(commande->commande);
	// printf("commande : %s\n", commande->commande);
	// printf("nb arg : %d\n", nbArgument);

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
		if (nbArgument >= 1)
		{
			listeArg =  initialisationString(premierArg(commande->commande));
			determinationArgs(listeArg, commande);
		}

		// Gestion particulière du cd
		if (!strcmp(substr(commande->commande,0,2), "cd"))
		{
			// Vérifie qu'il n'y a qu'un seul argument après le cd
			if (nbArgument == 1)
			{
				// Vérifie l'application du cd ../
				if (!strcmp(listeArg->premier->string, "..") || !strcmp(listeArg->premier->string, "../") || !strcmp(listeArg->premier->string, ".") || !strcmp(listeArg->premier->string, "./"))
				{
					if (!strcmp(listeArg->premier->string, ".") || !strcmp(listeArg->premier->string, "./"))
						printf("INTERDIT !!\n");
					else if (commande->niveau > 0)
					{
						chdir("../");
						remonterDossier(commande);
						commande->niveau = commande->niveau - 1;
					}else
						printf("INTERDIT !!\n");
				}
				// Vérifie que le dossier visé existe et qu'il n'est pas un fichier
				else if (dirExists(listeArg->premier->string))
				{
					// Vérifie que ce n'est pas un fichier
					if(fileExists(listeArg->premier->string))
						printf("\"%s\" est un fichier !!\n", listeArg->premier->string);
					else{
						chdir(listeArg->premier->string);
						sprintf(commande->directory, "%s/%s", commande->directory, listeArg->premier->string);
						commande->niveau = commande->niveau + 1;
					}
				}else
					printf("Le répertoire \"%s\" n'existe pas !!\n", listeArg->premier->string);
			}else
				printf("La commande \"cd\" prend un argument !!\n");
		}
		// Commandes sans argument
		else if (nbArgument == 0)
		{
			int status;
			int pid = fork();
		    if (pid == -1)
		    {
		        fprintf(stderr, "Erreur fork\n");
		        exit(EXIT_FAILURE);
		    }
		    if(pid == 0)
		    {
		    	execlp(commande->commande, commande->commande, NULL);
		        exit(0);
		    }else
		    {
		        waitpid(pid, &status, WCONTINUED);
		        if (WIFEXITED(status))
		        {
		              
		        }
		    }
		}
		// commandes autres que cd avec argument(s)
		else
		{
			int status;
			int pid = fork();
		    if (pid == -1)
		    {
		        fprintf(stderr, "Erreur fork\n");
		        exit(EXIT_FAILURE);
		    }
		    if(pid == 0)
		    {
		    	char **tab = malloc(sizeof(char)*TAILLE_MAX_COMMANDE*nbArgument);
		    	creeTabArgs(tab, listeArg, nbArgument);
		        execvp(tab[0], tab);
		        // printf("execution de %s\n", commande->commande);
		        exit(0);
		    }else
		    {
		        waitpid(pid, &status, WCONTINUED);
		        if (WIFEXITED(status))
		        {
		              
		        }
		    }
		}
	}
}

void debutLigne(Commande *commande){
	fprintf(stderr,"%s > ", commande->directory);
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