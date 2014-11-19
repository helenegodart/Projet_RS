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
	fichier = fopen(path,  "r+");
 
	if (fichier == NULL)
	    return 0;
	else
		return 1;
}

int dirExists(char *path){
  struct stat  file_stat; 
  if (stat(path,&file_stat) < 0)
    return 0;
  else 
    return 1;
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

int appartient(char *chaine, ListeString *liste){
  String *temp = liste->premier;
  if(!strcmp(chaine, temp->string))
    return 1;
  while((temp = temp->suivant) != NULL){
    if (!strcmp(chaine, temp->string))
      return 1;
  }
  return 0;
}

int search(char *chaine, char c){
  int i;
  for (i = 0; i < strlen(chaine); ++i)
  {
    if (chaine[i] == c)
      return i;
  }
  return 0;
}

void remonterDossier(Commande *commande){
  printf("remonterDossier\n");
  int indice = search(commande->directory, '/');
  commande->directory = substr(commande->directory, 0, strlen(commande->directory)-indice-2);
}

void creeTabArgs(char **tab, ListeString *liste, int nbArguments){
  int i = 0;
  int nb = nbArguments +1 ;
  String *temp = liste->premier;

  tab[nb-1] = temp->string;
  while((temp = temp->suivant) != NULL)
  {
    i++;
    tab[nb-1-i] = temp->string;
  }
}

int incrementNiveau(Commande *commande){
  int cpt = 0;

  char *chaine = malloc(sizeof(char)*strlen(commande->commande));
  char *temp = malloc(sizeof(char)*strlen(commande->commande));

  strcpy(chaine, commande->commande);

  // Si on commence à partir de la racine, interdire
  if (!strcmp(substr(chaine, 3, 4), "/"))
    return -1000;

  int i, length = strlen(chaine);
  temp = strtok(chaine, "/");
  if(!(!strcmp(substr(temp, 3, strlen(temp)-1), ".."))){
    cpt++;
    // printf("%s|\n", substr(temp, 3, strlen(temp)-1));
  }
  else if (!strcmp(substr(temp, 3, strlen(temp)-1), ".."))
  {
    cpt--;
    // printf("%s|\n", substr(temp, 3, strlen(temp)-1));
  }
  while((temp = strtok(NULL, "/")) != NULL){
    // printf("%s|\n", temp);
    if (!(!strcmp(temp, "..")))
      cpt++;
    else if(!strcmp(temp, "..")){
      cpt--;
    }
  }
  return cpt;
}

int accessible(Commande *commande){
  if (commande->niveau+incrementNiveau(commande) >=0)
    return 1;
  else
    return 0;
}

void fixDirectory(Commande *commande, Niveau *niveau){
  commande->directory = malloc(TAILLE_MAX_COMMANDE*sizeof(char));
  char *temp = malloc(TAILLE_MAX_COMMANDE*sizeof(char));

  getcwd(commande->directory, TAILLE_MAX_COMMANDE);
  temp = strstr(commande->directory, niveau->nom);
  strcpy(commande->directory, temp);
}

void pwd(Commande *commande){
  printf("%s\n", commande->directory);
}

void goBackRoot(Commande *commande){
  int i, count = commande->niveau;
  for (i = 0; i < count; ++i)
  {
    chdir("..");
  }
}