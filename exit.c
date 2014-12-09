#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include "niveau.h"
#include "utilitaire.h"
#include "terminal.h"

void ifExit(Commande *commande){

char *temp = malloc(sizeof(char)*strlen(commande->commande));
	if (strcmp(commande->commande, "exit")==0){
		endwin();
		exit(EXIT_SUCCESS);		
		}
}