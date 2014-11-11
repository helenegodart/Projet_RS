#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
		debutLigne(commande);
		fgets(commande->commande, TAILLE_MAX_COMMANDE, stdin);
		commande->commande = substr(commande->commande, 0, strlen(commande->commande)-1	);
		printf("%s\n", commande->commande);
		}
	return 0;
}

void execution(Commande *commande){

}

void debutLigne(Commande *commande){
	fprintf(stderr,"%s > ", commande->directory);
}

// void getLigne(char *commande){
//     // fgets(commande, TAILLE_MAX_COMMANDE, stdin);
//     // clean(commande, stdin);
//     printf("getligne\n");
//     char *temp;
//     scanf("%s",temp);
//     getchar();
//     fprintf(stderr,"len : %d\n", strlen(temp));
//     commande = malloc(strlen(temp)*sizeof(char));
//     printf("malloc\n");
//     strcpy(commande, temp);
// }
	 
void clean(const char *buffer, FILE *fp){
    char *p = strchr(buffer,'\n');
    if (p != NULL)
        *p = 0;
    else{
	    int c;
	    while ((c = fgetc(fp)) != '\n' && c != EOF);
    }
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
	c->commande = malloc(sizeof(c));
	c->directory = malloc(sizeof(c));
}