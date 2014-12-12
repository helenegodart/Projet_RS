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
		removeDirectory(niveau);
		exit(EXIT_SUCCESS);		
		}
}

void removeDirectory(Niveau *niveau){
    char *pwd = malloc(sizeof(char)*TAILLE_MAX_COMMANDE);
    getcwd(pwd, TAILLE_MAX_COMMANDE);
    if (strstr(pwd, niveau->nom) == NULL)
    {
    	fprintf(stderr, "ERREUR impossible de supprimer le dossier %s\n", niveau->nom);
    }
    else
    {
    	fprintf(stderr, "strstr : %s, niveau->nom : %s\n", strstr(pwd, niveau->nom), niveau->nom);
	    while(strcmp(strstr(pwd, niveau->nom), niveau->nom) != 0){
	        chdir("..");
	        getcwd(pwd, TAILLE_MAX_COMMANDE);
	    }
	    chdir("..");
	    rmdir_recursive(niveau->nom);
	}
}

void rmdir_recursive(char *directoryPath){
    DIR* dp; 
    struct dirent *ep; 
    char buffer[1024]; 
    struct stat st; 
    char* temp;  
    dp = opendir(directoryPath);  
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
}