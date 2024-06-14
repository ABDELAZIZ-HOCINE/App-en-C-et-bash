#include <stdio.h>
#include <stdlib.h>

// fonction qui prends un cara est le representer en binaire
void afficherBinaire(char c) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (c >> i) & 1);
    }
}

// fonction qui va decoder le fichier encoder et va lui mettre en fichier sortie
void decoderFichier(char *fichierEncode, char *fichierBrut) {
    FILE *fichierEntree, *fichierSortie;
    char buffer[1024];
    size_t bytesRead;
    //ouvrir les fichiers 
    fichierEntree = fopen(fichierEncode, "rb");
    fichierSortie = fopen(fichierBrut, "wb");
    // verifier si le fichier est vide ou bien une erreur d'ouverture 
    if (fichierEntree == NULL || fichierSortie == NULL) {
        perror("Erreur lors de l'ouverture des fichiers");
        exit(EXIT_FAILURE);
    }

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), fichierEntree)) > 0) {
        //utiliser fwrite pour ecrire le contenu du buffer dans le fichier de sortie
        fwrite(buffer, 1, bytesRead, fichierSortie);
    }

    fclose(fichierEntree);
    fclose(fichierSortie);
}

int main(int argc, char *argv[]) {
    //ouvrir le fichier
    FILE *fichier = fopen(argv[1], "rb");

    // verifier si le fichier est vide ou bien une erreur d'ouverture
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 2;
    }

    char *caracteresPairs = NULL;
    char *caracteresImpairs = NULL;
    int taillePairs = 0;
    int tailleImpairs = 0;
    char buffer[1024];
    size_t bytesRead; // Declaration de bytesRead

    printf("Contenu du fichier en binaire avec les 3 bits de poids forts à 0 :\n");

    int position = 0; // Declaration de position

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), fichier)) > 0) {
        for (size_t i = 0; i < bytesRead; i++) {
            afficherBinaire(buffer[i]);
            printf("\n");

            // Modification des 3 bits de poids forts à 0
            buffer[i] &= 0x1F;

            // Allocation dynamique pour les tableaux pairs et impairs
            if (position % 2 == 0) {
                caracteresPairs = realloc(caracteresPairs, (taillePairs + 1) * sizeof(char));
                caracteresPairs[taillePairs++] = buffer[i];
            } else {
                caracteresImpairs = realloc(caracteresImpairs, (tailleImpairs + 1) * sizeof(char));
                caracteresImpairs[tailleImpairs++] = buffer[i];
            }

            position++;
        }
    }

    // Remplir le dernier groupe d'octets si necessaire
    int resteoctets = 5 - (position % 5);

    if (resteoctets != 5 && resteoctets > 0) {
        printf("\nRemplir le dernier groupe avec %d octets\n", resteoctets);

        // Ajouter l'octet 10000000 pour le premier groupe
        afficherBinaire(0x80);
        if (position % 2 == 0) {
            caracteresPairs = realloc(caracteresPairs, (taillePairs + 1) * sizeof(char));
            caracteresPairs[taillePairs++] = 0x80;
        } else {
            caracteresImpairs = realloc(caracteresImpairs, (tailleImpairs + 1) * sizeof(char));
            caracteresImpairs[tailleImpairs++] = 0x80;
        }

        position++;

        // Remplir le reste avec 00000000
        for (int i = 1; i < resteoctets; i++) {
            afficherBinaire(0x00);
            if (position % 2 == 0) {
                caracteresPairs = realloc(caracteresPairs, (taillePairs + 1) * sizeof(char));
                caracteresPairs[taillePairs++] = 0x00;
            } else {
                caracteresImpairs = realloc(caracteresImpairs, (tailleImpairs + 1) * sizeof(char));
                caracteresImpairs[tailleImpairs++] = 0x00;
            }

            position++;
        }
    }

    fclose(fichier);

    // Afficher les caracteres classes
    printf("\n\nCaracteres pairs : \n");
    for (int i = 0; i < taillePairs; i++) {
        afficherBinaire(caracteresPairs[i]);
        printf(" \n");
    }

    printf("\nCaracteres impairs : \n");
    for (int i = 0; i < tailleImpairs; i++) {
        afficherBinaire(caracteresImpairs[i]);
        printf(" \n");
    }

    decoderFichier(argv[1], argv[2]);

    // Liberer la memoire allouee dynamiquement
    free(caracteresPairs);
    free(caracteresImpairs);

    return 0;
}