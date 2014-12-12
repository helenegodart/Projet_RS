#define TAILLE_MAX_COMMANDE 500

typedef struct Commande Commande;
struct Commande{
	char *directory;
	char *commande;
	int niveau;
	int pid;
};

typedef struct String String;
struct String
{
	char *string;
	String *suivant;
};

typedef struct ListeString ListeString;
struct ListeString
{
	String *premier;
};

typedef struct Niveau Niveau;
struct Niveau
{
	char *nom;
	ListeString *charAutorise;
	char *phraseMystere;
	char *consigne;
	ListeString *history;
};

// Initialise une liste de String
void initialisationString(ListeString *liste, char *c);

// Crée un niveau à partir du fichier passé en paramètre
void creerNiveau(char *path, Niveau *niveau, char *nomNiveau);

// Insère une nouvelle chaine de caractère dans une liste chainee liste
void insertionString(ListeString *liste, char *string);

// Décompresse l'archive tar.gz du niveau et le crée 
void decompression(char *nom, Commande *commande, Niveau *niveau);

// Copier un fichier
int copier_fichier(char *source, char *destination);
