
// Renvoie une sous chaine de la chaine passée en argument
char *substr(char *src,int pos,int len);	

// Test si le fichier existe
int fileExists(char *path);

//Retourne le nombre d'argument d'une commande utilisateur
int nbArg(Commande *commande);

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

// Renvoie le nb d'avancées par cd /dossier1/dossier2 => 2
int incrementNiveau(Commande *commande);

// Renvoie un boolean pour savoir si on peut accéder au dossier (utilise la fonction au-dessus)
int accessible(Commande *commande);

// Remet à jour le directory
void fixDirectory(Commande *commande, Niveau *niveau);

// Affiche le working directory
void pwd(Commande *commande);

// Retourne à la racine du niveau
void goBackRoot(Commande *commande);

// Construit la liste chaînée qui contient les arguments
void buildArgsChain(ListeString *liste, Commande *commande);