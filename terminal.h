

// Ecrit le working directory plus ">"
void debutLigne(Commande *commande);

// 
void clean(const char *buffer, FILE *fp);

// Récupère le texte tapé par l'utilisateur
void getLigne(char *chaine);

// printf le descriptif du niveau : commandes autorisée + but à atteindre
void descriptifNiveau(Niveau *niveau);

// Execute la commande lancée par l'utilisateur
void execution(Commande *commande);

// 
void initialiseCommande(Commande *c);