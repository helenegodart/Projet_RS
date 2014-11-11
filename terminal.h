

typedef struct Commande Commande;
struct Commande{
	char *directory;
	char *commande;
};

// Ecrit le working directory plus ">"
void debutLigne();

// 
void clean(const char *buffer, FILE *fp);

// Récupère le texte tapé par l'utilisateur
void getLigne(char *chaine);

// printf le descriptif du niveau : commandes autorisée + but à atteindre
void descriptifNiveau(Niveau *niveau);

// Execute la commande lancée par l'utilisateur
void execution(Commande *commande);