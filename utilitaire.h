
// Renvoie une sous chaine de la chaine passée en argument
char *substr(char *src,int pos,int len);	

// Test si le fichier existe
int fileExists(char *path);

//Retourne le nombre d'argument d'une commande utilisateur
int nbArguments(char *commande);

// Test si le dossier existe 
int dirExists(char *path);

// Retourne le premier argument d'une commande utilisateur (pour initialisation de la liste de commande)
char *premierArg(char *commande);

// supprimer le retour à la ligne d'une saisie
void clean (char *chaine);

// Vider le buffer
void purger();

// Teste si une chaine appartient à une liste chaînée
int appartient(char *chaine, ListeString *liste);

// recherche le caractère dans la chaine et renvoie l'indice
int search(char *chaine, char c);

// niveau1/dossier1/ devient niveau1/
void remonterDossier(Commande *commande);

//Crée un tableau avec les arguments
void creeTabArgs(char **tab, ListeString *liste, int nbArguments);