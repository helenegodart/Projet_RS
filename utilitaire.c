#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utilitaire.h"
#include <errno.h>

char *substr(char *src,int pos,int len) { 
  char *dest=NULL;                        
  if (len>0) {                          
    dest = calloc(len+1, 1);       
    if(NULL != dest)
        strncat(dest,src+pos,len);            
  }                                       
  return dest;                            
}

int fileExists(char *path){
	FILE *fichier = NULL;
	fichier = fopen(path,  "r");
 
	if (fichier == NULL)
	    return 1;
	else
		return 0;
}

int dirExists(char *path){

}

int nbArg(char *commande){
	int nb = 0;
	char *temp = malloc((strlen(commande))*sizeof(char));
	strcpy(temp, commande);
	strtok(temp, " ");
	while(strtok(NULL, " ") != NULL)
		nb = nb +1;

	return nb;
}

char *premierArg(char *commande){
	char *temp = malloc((strlen(commande))*sizeof(char));
	strcpy(temp, commande);
	return strtok(temp, " ");
}

void purger(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {}
}

void clean (char *chaine){
    char *p = strchr(chaine, '\n');
    if (p)
    {
        *p = 0;
    }else
    {
        purger();
    }
}