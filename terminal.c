#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "niveau.h"
#include "terminal.h"

int main(int argc, char const *argv[])
{
	int continuer = 1;

	char commande[20] = "";
	Commande *commande;
	commande = initialiseCommande();

	Niveau niveau;
	creerNiveau("meta", &niveau);
	descriptifNiveau(&niveau);

	while(continuer){
		debutLigne();
		getLigne(commande);
		printf("%s\n", commande);
		}
	return 0;
}

void execution(Commande *commande){

}

void debutLigne(){

	fprintf(stderr,"> ");
}

void getLigne(char *commande){
    fgets(commande, 50, stdin);
    clean(commande, stdin);
}
	 
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

void initialiseCommande(){
	
}