#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
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

void redirection(Niveau *niveau, Commande *commande, int mode){	
	if (mode == ENVOI_W)
		writeFile(niveau, commande, ">>");
	else if (mode == ENVOI_WR)
		writeFile(niveau, commande, ">");
	else if(mode == RECOI_WR)
		readFile(niveau, commande, "<");

}

void writeFile(Niveau *niveau, Commande *commande, char *token){
	char *fichier = malloc(sizeof(char)*strlen(commande->commande));
	Commande *newCommande = malloc(sizeof(newCommande));
	strcpy(fichier, findFileName(fichier, commande, newCommande, token));
	
	FILE *f = NULL;

	if(!strcmp(token, ">>"))
		f = fopen(fichier, "a");
	else if(!strcmp(token, ">"))
		f = fopen(fichier, "w");

	
	

	if (f == NULL)
	{
		printw("erreur ouverture %s\n", fichier);
		endwin();
		exit(EXIT_FAILURE);
	}else{
		ListeString *listeArg = malloc(sizeof(ListeString));
		buildArgsChain(listeArg, newCommande);
		char *result = exec(listeArg, newCommande, niveau);
		fputs(result, f);
		fclose(f);
		free(listeArg);
	}
	free(fichier);
	free(newCommande);
}

void readFile(Niveau *niveau, Commande *commande, char *token){
	printw("RECOI_W\n");
	char *ligne = malloc(sizeof(char)*strlen(commande->commande));
	char *com = malloc(sizeof(char)*strlen(commande->commande));
	char *file = malloc(sizeof(char)*strlen(commande->commande));

	Commande *newCommande = malloc(sizeof(newCommande));
	initialiseCommande(newCommande);
	strcpy(newCommande->directory, commande->directory);
	newCommande->niveau = commande->niveau;
	
	strcpy(ligne, commande->commande);
	ligne = deleteSpaces(ligne);
	com = strtok(ligne, "<");
	file = strtok(NULL, "<");
	sprintf(ligne, "%s %s", com, file);
	wprintw(stdscr, "com : |%s| ; file : |%s|\ncommande : %s\n",com, file, ligne );
	// strcat(fichier, findFileName(fichier, commande, newCommande, token));

	strcpy(newCommande->commande, com);
	if (!fileExists(file))
	{
		wprintw(stdscr, "fichier \"%s\" inexistant !!\n", file);
	}
	else
	{
		
		ListeString *listeArg = malloc(sizeof(ListeString));
		buildArgsChain(listeArg, newCommande);
		wprintw(stdscr, "commande, %s\n", newCommande->commande);
		wprintw(stdscr, "%s\n", exec(listeArg, newCommande, niveau));
		free(ligne);
	}
	free(com);
	free(file);
	free(newCommande);	
}

char *findFileName(char *fichier, Commande *commande, Commande *newCommande, char *token){
	char *ligne = malloc(sizeof(char)*strlen(commande->commande));
	strcpy(ligne, commande->commande);
	char *temp = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
	char *com = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
	com = strtok(ligne, token);
	fichier = strtok(NULL, token);
	fichier = deleteSpaces(fichier);

	initialiseCommande(newCommande);
	strcpy(newCommande->commande, com);
	strcpy(newCommande->directory, commande->directory);
	newCommande->niveau = commande->niveau;

	return fichier;
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

char *choixNiveau(int argc, char *argv[]){
	if (argc != 2)
	{
		printw("Il faut donner en argument (seulement) l'archive du niveau !\n");
		endwin();
		exit(EXIT_FAILURE);
	}else{
		char *temp = malloc(strlen(argv[1])*sizeof(char));
		char *tar, *gz;
		strcpy(temp, argv[1]);
		char *nom = malloc(strlen(argv[1])*sizeof(char));
		nom = strtok(temp, ".");
		tar = strtok(NULL, ".");
		gz = strtok(NULL, ".");
		if ((!strcmp(tar, "tar") && !strcmp(gz, "gz")) || !strcmp(tar, "tgz"))
			return nom;
		else{
			printw("Il faut une archive en .tar.gz ou en .tgz !!\n");
			endwin();
			exit(EXIT_FAILURE);
		}
	}
}

void verification(char *sortie, Niveau *niveau){
	char *test = malloc(sizeof(char)*strlen(sortie));

	if((test = strstr(sortie, niveau->phraseMystere)) != NULL){
		printw("NIVEAU REUSSI !!\n");
		endwin();
		exit(EXIT_SUCCESS);	
	}
}

char *autoComplete(char *saisie, Niveau *niveau){
	//Elaboration de la liste chainee de propositions
	ListeString *liste = malloc(sizeof(ListeString));
	String *temp = malloc(sizeof(String));
	temp = niveau->charAutorise->premier;
	initialisationString(liste, temp->string);
	while((temp = temp->suivant) != NULL){
		insertionString(liste, temp->string);
	}
	//appel de la méthode exec pour le ls
	int fd[2];
	pipe(fd);
	int status;
	char *ls = malloc(sizeof(char)*TAILLE_MAX_COMMANDE*200);
	int pid = fork();
	if(pid == 0)
    {
		close(fd[0]);
	   	dup2(fd[1], 1);
	   	close(fd[1]);
		execlp("ls", "ls", NULL);
	    exit(0);
    }else
    {
        waitpid(pid, &status, WCONTINUED);
        if (WIFEXITED(status))
        {
        	read(fd[0], ls, TAILLE_MAX_COMMANDE*200);
        }
    }
    char *t = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
    t = strtok(ls, "\n");
    insertionString(liste, t);
    while((t = strtok(NULL, "\n")) != NULL)
    	insertionString(liste, t);

    temp = malloc(sizeof(String));
    temp = liste->premier;
    // printw("- %s\n", temp->string);
    // while((temp = temp->suivant) != NULL)
    // 	printw("- %s\n", temp->string);
    int continuer = 1;

    char *dernierMot = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
    char *saisieCpy = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
    char *test = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
    strcpy(saisieCpy, saisie);
    // printw("dernierMot : |%s|", (dernierMot = strtok(saisieCpy, " ")));
    if((dernierMot = strtok(saisie, " ")) != NULL){
    	// printw("dernierMot : %s\n", dernierMot);
    	strcpy(test, dernierMot);
    	while((dernierMot = strtok(NULL, " ")) != NULL){
    		// printw("dernierMot : %s\n", dernierMot);
    		strcpy(test, dernierMot);
    	}
    }
    else {
    	strcpy(dernierMot, saisie);
    }
    // printw("dernierMot : %s\tsaisieCpy : %s\ttest : %s\n", dernierMot, saisieCpy, test);
    /** Cas à traiter :
    	une lettre est incluse dans le mot mais n'est pas le commencement (a dans cat)
    	deux fichiers qui commencent pareil, n'afficher que ce qui est commun aux deux
    */
    int match = 0, x, y;

    while(continuer){
    	if ((temp = temp->suivant) == NULL)
    		continuer = 0;
    	else if(!strcmp(substr(temp->string, 0, strlen(test)), test)){

    		strcpy(test, temp->string);
    		continuer = 0;
    		match = 1;
    	}
    }
    if(match)
		return test;
	else
		return "";
}

void ifTab(Commande *commande){
	if (strstr(commande->commande, "\t") != NULL)
	{
		wprintw(stdscr, "ifTab\n");
	}
}
