#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utilitaire.h"
#include "niveau.h"
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
		// commande->commande = substr(commande->commande, 0, strlen(commande->commande)-1	);
		strcpy(commande->commande, saisie);
		execution(commande);
		}
	return 0;
}

void execution(Commande *commande){
	int nbArgument = nbArg(commande->commande);
	printf("commande : %s\n", commande->commande);
	printf("nb arg : %d\n", nbArgument);

	ListeString *listeArg = malloc(sizeof(ListeString));
	if (nbArgument >= 1)
	{
		// char *arg1 = ;
		listeArg =  initialisationString(premierArg(commande->commande));
		determinationArgs(listeArg, commande);
	}

	// Gestion particulière du cd
	if (!strcmp(substr(commande->commande,0,2), "cd"))
	{
		
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
	printf("temp : %s\n", temp);
	arg = strtok(temp, " ");
	while((arg = strtok(NULL, " ")) != NULL)
	{
		printf("insertion de : %s\n", arg);
		insertionString(*(&liste), arg); 
	}
}