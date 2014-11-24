#define ENVOI_W 1
#define ENVOI_WR 2
#define RECOI_W 3
#define RECOI_WR 4

// Teste si il y a une commande >>
int isRedirector(Commande *commande);

// Gère la commande >>
void redirection(Niveau *niveau, Commande *commande, int mode);

// Supprime espaces
char *deleteSpaces(char *chain);

// Write in a file (command >> || <<)
void writeFile(Niveau *niveau, Commande *commande, char *token);

// Ouvrir le niveau donné en argument de la ligne de commande + vérifications
char *choixNiveau(int argc, char *argv[]);

// Vérifie que la sortie est bien la phrase à trouver
void verification(char *sortie, Niveau *niveau);