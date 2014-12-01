#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

int
remove_directory(char const *name)
{
    DIR *directory;           /* pointeur de répertoire */
    struct dirent *entry;     /* représente une entrée dans un répertoire. */
    struct stat file_stat;    /* informations sur un fichier. */

    /* Ce tableau servira à stocker le chemin d'accès complet
     * des fichiers et dossiers. Pour simplifier l'exemple,
     * je le définis comme un tableau statique (avec une taille
     * a priori suffisante pour la majorité des situations),
     * mais on pourrait l'allouer dynamiquement pour pouvoir
     * le redimensionner si jamais on tombait sur un chemin
     * d'accès démesurément long. */
    char buffer[1024] = {0};

    /* On ouvre le dossier. */
    directory = opendir(name);
    if ( directory == NULL ) {
        fprintf(stderr, "cannot open directory %s\n", name);
        return 0;
    }

    /* On boucle sur les entrées du dossier. */
    while ( (entry = readdir(directory)) != NULL ) {

        /* On "saute" les répertoires "." et "..". */
        if ( strcmp(entry->d_name, ".") == 0 ||
             strcmp(entry->d_name, "..") == 0 ) {
            continue;
        }

        /* On construit le chemin d'accès du fichier en
         * concaténant son nom avec le nom du dossier
         * parent. On intercale "/" entre les deux.
         * NB: '/' est aussi utilisable sous Windows
         * comme séparateur de dossier. */
        snprintf(buffer, 1024, "%s/%s", name, entry->d_name);

        /* On récupère des infos sur le fichier. */
        stat(buffer, &file_stat);
        /* J'ai la flemme de tester la valeur de retour, mais
         * dans un vrai programme il faudrait le faire :D */

        if ( S_ISREG(file_stat.st_mode) ) {
            /* On est sur un fichier, on le supprime. */
            remove(buffer);
        }
        else if ( S_ISDIR(file_stat.st_mode) ) {
            /* On est sur un dossier, on appelle cette fonction. */
            remove_directory(buffer);
        }
    }

    /* On ferme le dossier. */
    closedir(directory);

    /* Maintenant le dossier doit être vide, on le supprime. */
    remove(name);
    /* J'ai toujours la flemme de tester la valeur de retour... */

    return 1;
}