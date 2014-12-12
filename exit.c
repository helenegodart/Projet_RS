#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ncurses.h>
#include "niveau.h"
#include "utilitaire.h"
#include "terminal.h"
#include "exit.h"

void ifExit(Niveau *niveau, Commande *commande){

char *temp = malloc(sizeof(char)*strlen(commande->commande));
	if (strcmp(commande->commande, "exit")==0){
		endwin();
		removeDirectory(niveau->nom);
        freeAll(niveau, commande);
		exit(EXIT_SUCCESS);		
		}
}

void removeDirectory(char *nom){
    fprintf(stderr, "__nom : %s\n", nom);
    char *nomDirectory = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
    // strcpy(nomDirectory, nom);
    strcpy(nomDirectory, nom);
    fprintf(stderr, "removeDirectory\n");
    fprintf(stderr, "_nom : %s, nomDirectory : %s\n", nom, nomDirectory);
    char *pwd = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
    getcwd(pwd, TAILLE_MAX_COMMANDE);//malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
    fprintf(stderr, "malloc\n");
    // getcwd(pwd, TAILLE_MAX_COMMANDE);
    fprintf(stderr, "getcwd\n");
    fprintf(stderr, "__pwd : %s\n", pwd);
    // fprintf(stderr, "niveau = NULL : %d\n", niveau == NULL);
    // fprintf(stderr, "niveau->nomDirectory = NULL : %d\n", niveau->nomDirectory == NULL);
    // fprintf(stderr, "taille de nomDirectory : %d\n", strlen(niveau->nomDirectory));
    fprintf(stderr, "nomDirectory : %s\n", nomDirectory);
    fprintf(stderr, "coucou\n");
    // fprintf(stderr, "%s\n", );
    if (strstr(pwd, nomDirectory) == NULL)
    {
    	fprintf(stderr, "ERREUR impossible de supprimer le dossier %s\n", nomDirectory);
    }
    else
    {
        fprintf(stderr, "avant while\n");
        fprintf(stderr, "pwd : %s, nomDirectory : %s\n", pwd, nomDirectory);
	    while(strcmp(strstr(pwd, nomDirectory), nomDirectory) != 0){
	        chdir("..");
	        getcwd(pwd, TAILLE_MAX_COMMANDE);
	    }
	    chdir("..");
        fprintf(stderr, "placé ds le bon répertoire\n");
	    rmdir_recursive(nomDirectory);
	}
    free(pwd);
    free(nomDirectory);
}

void rmdir_recursive(char *directoryPath){
    fprintf(stderr, "rmdir_recursive\n");
    DIR* dp; 
    struct dirent *ep; 
    char buffer[1024]; 
    struct stat st; 
    char* temp;  
    dp = opendir(directoryPath);  
    fprintf(stderr, "opendir\n");
    if (dp != NULL) { 
    	while( (ep = readdir(dp) ) != NULL) { 
    		sprintf(buffer, "%s/%s", directoryPath,ep->d_name); 
    		lstat(buffer, &st);  
    		if(S_ISDIR(st.st_mode)) { 
    			if(strcmp(".",(ep->d_name)) != 0 && strcmp("..",(ep->d_name)) != 0) { 
    				rmdir_recursive(buffer); 
    			} 
    		}  
    		else { 
    			unlink (buffer); 
    		}  
    	} 
    } 
    rmdir(directoryPath); 
    free(dp);
    free(ep);
}