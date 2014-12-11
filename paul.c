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
	// Si la chaine est vide faire un ls
	if(strlen(saisie) == 0){
		printw("\n%s", ls());
		return "";
	}
	//Elaboration de la liste chainee de propositions
	ListeString *liste = malloc(sizeof(ListeString));
	String *temp = malloc(sizeof(String));
	temp = niveau->charAutorise->premier;
	initialisationString(liste, temp->string);
	while((temp = temp->suivant) != NULL){
		insertionString(liste, temp->string);
	}
	//appel de la méthode exec pour le ls
	char *lsResult = malloc(sizeof(char)*TAILLE_MAX_COMMANDE*200);
	lsResult = ls();
    char *t = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
    t = strtok(lsResult, "\n");
    insertionString(liste, t);
    while((t = strtok(NULL, "\n")) != NULL)
    	insertionString(liste, t);

    temp = malloc(sizeof(String));
    temp = liste->premier;
    // printw("- %s\n", temp->string);
    // while((temp = temp->suivant) != NULL)
    // 	printw("- %s\n", temp->string);

    char *test = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
    int continuer = 1;
    // Sélectionne le dernier mot tapé si il y a des espaces
	char *dernierMot = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
	char *saisieCpy = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
	strcpy(saisieCpy, saisie);

    if(strstr(saisie, " ") != NULL){
    	printw("espace\n");
	    // printw("dernierMot : |%s|", (dernierMot = strtok(saisieCpy, " ")));
	    if((dernierMot = strtok(saisieCpy, " ")) != NULL){
	    	// printw("dernierMot : %s\n", dernierMot);
	    	strcpy(test, dernierMot);
	    	while((dernierMot = strtok(NULL, " ")) != NULL){
	    		// printw("dernierMot : %s\n", dernierMot);
	    		strcpy(test, dernierMot);
	    	}
	    }
	    else {
	    	strcpy(dernierMot, saisieCpy);
	    }
	    strcpy(saisieCpy, test);
	}
	
    // Construit une liste de toutes les possibilités qui match
    
    ListeString *results = malloc(sizeof(ListeString));
    int match = 0, x, y;
    continuer = 1;
    free(test);
    test = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
    strcpy(test, saisieCpy);
    // printw("test : %s\n", test);
    while(continuer){
    	if ((temp->suivant) != NULL){
    		temp = temp->suivant;
			if(!strcmp(substr(temp->string, 0, strlen(test)), test) ){
				// strcpy(test, substr(temp->string, strlen(test), strlen(temp->string)));
				// printw("test : %s _ %s\n", substr(temp->string, 0, strlen(test)), test);
				if(!match){
					// printw("initialisation : %s\n", temp->string);
					initialisationString(results, temp->string);
				}
				else{
					// printw("insertion : %s\n", temp->string);
					insertionString(results, temp->string);
				}
				match = 1;
    		}
    	}else{
    		continuer = 0;
    	}
    }


    // Complète la saisie jusqu'à la première ambiguïté
  
    // printw("results : \n");
    free(temp);
    temp = malloc(sizeof(String));
    // test = malloc(sizeof(String));
    
    int fin = 0, i = 0;
    // printw("results :\n");
    // printw("\t %s\n", results->premier->string);
    // temp = results->premier;
    // while((temp = temp->suivant) != NULL)
    // 	printw("\t %s\n", temp->string);
    free(test);
    test = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
    strcpy(test, "");
    // printw("saisie : %s", saisie);
    free(temp);
    temp = malloc(sizeof(String));
    while(!fin){
    		if(temp->suivant == NULL)
    			temp->suivant = results->premier;
    		// fprintf(stderr, "suivant null ? %d\n", temp->suivant == NULL);
    		temp = temp->suivant;
    		// fprintf(stderr, "tête ? %d\n", temp == results->premier);
    		// printw("__test : %s\n", test);
    		if(temp == results->premier){
    			i++;
    			// fprintf(stderr, "valeur de i : %d, ajout : %s\n", i, results->premier->string);
			    // printw("_test : %s + %s - %d\n", test, substr(results->premier->string, i, i), i);
    			// strcat(test, substr(results->premier->string, i, i));
    			sprintf(test, "%s%c", test, results->premier->string[i]);
    			
    		}
    		// fprintf(stderr, "recherche du char, %s\n", strstr(temp->string, test));
    		// fprintf(stderr, "premier : %s\n", results->premier->string);
    		if(strstr(temp->string, test) == NULL || i > strlen(results->premier->string)){
    			// fprintf(stderr, "condition finale, i=%d, test = %s\n", i, test);
    			// printw("test : %s\n", test);
    			sprintf(test, "%s", substr(test, 0, strlen(test)-1));
    			fin = 1;
    		}
	    	// strcpy(test, strstr(results->premier->string, temp->string));
	    	// printw("t : %s\nt : %s\ncomposé : %s", results->premier->string, temp->string, strstr(results->premier->string, temp->string));
    	// }else
    	// 	temp->suivant = results->premier;
    }
    

    // free(liste);
    // free(temp);
    // free(ls);
    // free(t);
    // free(dernierMot);
    // free(saisieCpy);
    // free(results);
    // printw("\ntest : %s\n", test);
    if(match)
		return substr(test,strlen(saisie)-1,strlen(test))/*test/**/;
	else
		return "";
}

void ifTab(Commande *commande){
	if (strstr(commande->commande, "\t") != NULL)
	{
		wprintw(stdscr, "ifTab\n");
	}
}

char *ls(){
	char *ls = malloc(sizeof(char)*TAILLE_MAX_COMMANDE*200);
	int fd[2];
	pipe(fd);
	int status;
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
    return ls;
}