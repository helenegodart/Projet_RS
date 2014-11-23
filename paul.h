#define ENVOI_W 1
#define ENVOI_WR 2
#define RECOI_W 3
#define RECOI_WR 4

// Teste si il y a une commande >>
int isRedirector(Commande *commande);

// Gère la commande >>
void redirection(Commande *commande, int mode);

// Supprime espaces
char *deleteSpaces(char *chain);

// Write in a file (command >> || <<)
void writeFile(Commande *commande, char *token);