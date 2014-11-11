#define TAILLE_MAX_COMMANDE 200

typedef struct Commande Commande;
struct Commande{
	char *directory;
	char *commande;
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
	ListeString *charAutorise;
	char *phraseMystere;
};

// Initialise une liste de String
ListeString *initialisationString(char *c);

// Crée un niveau à partir du fichier passé en paramètre
void creerNiveau(char *path, Niveau *niveau);

void insertionString(ListeString *liste, char *string);

// Renvoie une sous chaine de la chaine passée en argument
char *substr(char *src,int pos,int len);

// Décompresse l'archive tar.gz du niveau et le crée
void decompression(char *nom, Commande *commande, Niveau *niveau);