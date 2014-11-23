

// Ecrit le working directory plus ">"
void debutLigne(Commande *commande);

// printf le descriptif du niveau : commandes autorisée + but à atteindre
void descriptifNiveau(Niveau *niveau);

// Execute la commande lancée par l'utilisateur
void execution(Commande *commande, Niveau *niveau);

// Initialise la première commande
void initialiseCommande(Commande *c);

// remplie la liste chainée des arguments de la commande utilisateur
void determinationArgs(ListeString *liste, Commande *commande);

// test toutes les commandes/signaux particuliers (CTRL-C, exit...)
void exceptionProcessing(Commande *commande);

// Lance l'exécution d'une commande (appel execlp + fork + pipe..), renvoi la réponse
char *exec(ListeString *listeArg, Commande *commande);