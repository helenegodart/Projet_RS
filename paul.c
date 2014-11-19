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
		return 1;
	else
		return 0;
}

void redirection(Commande *commande){
	// int fd[2];
	// pipe(fd);
	// int status;
	// int pid = fork();
 //    if (pid == -1)
 //    {
 //        fprintf(stderr, "Erreur fork\n");
 //        exit(EXIT_FAILURE);
 //    }
 //    if(pid == 0)
 //    {
 //    	char **tab = malloc(sizeof(char)*TAILLE_MAX_COMMANDE*nbArgument);
 //    	creeTabArgs(tab, listeArg, nbArgument);
 //        execvp(tab[0], tab);
 //        exit(0);
 //    }else
 //    {
 //        waitpid(pid, &status, WCONTINUED);
 //        if (WIFEXITED(status))
 //        {
        	
 //        }
 //    }
}