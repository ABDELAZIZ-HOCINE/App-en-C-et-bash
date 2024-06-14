#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour convertir un octet en chaine binaire sur 8 bits
void convertToBinary(unsigned char octet, char *binary) {
    for (int i = 7; i >= 0; i--) {
        binary[7 - i] = ((octet >> i) & 1) + '0';
    }
    binary[8] = '\0'; // Terminateur de chaine
}

void decodeFile(char *fichierEntree) {
    FILE *file = fopen(fichierEntree, "rb");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }

    unsigned char octet;
    int indexPairs = 0;
    int indexImpairs = 0;
    unsigned char octetPairs[1024]; 
    unsigned char octetImpairs[1024];

    while (fscanf(file, "%hhu", &octet) != EOF) {
        octetPairs[indexPairs++] = octet;
        if (fscanf(file, "%hhu", &octet) != EOF) {
            octetImpairs[indexImpairs++] = octet;
        } else {
            break; 
        }
    }

    // Allouer de la memoire pour les tableaux binaires pairs et impairs
    //Alloue de la memoire pour deux tableaux pairs et impairs
    char *pairs = (char*)malloc(indexPairs* 9 * sizeof(char)); // 9eme pour terminateur
    char *impairs = (char*)malloc(indexImpairs * 9 * sizeof(char));

    if (pairs == NULL || impairs == NULL) {
        printf("Erreur d'allocation de memoire.\n");
        fclose(file);
        return;
    }

    // Fonction pour convertir un nombre decimal en binaire
    long decimalToBinary(int decimal) {
        long binary = 0;
        int remainder, base = 1;

        while (decimal > 0) {
            remainder = decimal % 2;
            binary = binary + remainder * base;
            decimal = decimal / 2;
            base = base * 10;
        }

        return binary;
    }

    // Fonction pour formater une chaine binaire sur 8 bits
    void formatBinaryToEightBits(char *binary) {
        int length = strlen(binary);

        if (length < 8) {
            int diff = 8 - length;
            char temp[9]; // Pour stocker le resultat temporairement

            // Remplir temp avec des zeros
            for (int i = 0; i < diff; i++) {
                temp[i] = '0';
            }
            temp[diff] = '\0'; // Terminateur de chaine

            // Concatener la chaine binaire originale apres les zeros
            strcat(temp, binary);
            strcpy(binary, temp); // Copier le resultat dans la chaine binaire d'origine
        }
    }


    // Convertir les octets pairs en chaines binaires et les stocker dans pairs
    for (int i = 0; i < indexPairs; i++) {
        char binary[9]; // 8 bits + 1 pour le terminateur
        long bin = decimalToBinary(octetPairs[i]); // Utilisation de decimalToBinary
        sprintf(binary, "%ld", bin); // Conversion de long en chaine de caracteres
        formatBinaryToEightBits(binary); // Formater la chaine binaire a 8 bits
        strcat(pairs, binary); // Concatener la chaine binaire a pairs
    }

    // Convertir les octets impairs en chaines binaires et les stocker dans impairs
    for (int i = 0; i < indexImpairs; i++) {
        char binary[9]; // 8 bits + 1 pour le terminateur
        long bin = decimalToBinary(octetImpairs[i]); // Utilisation de decimalToBinary
        sprintf(binary, "%ld", bin); // Conversion de long en chaine de caracteres
        formatBinaryToEightBits(binary); // Formater la chaine binaire a 8 bits
        strcat(impairs, binary); // Concatener la chaine binaire a impairs
    }


    // Affichage des tableaux reconstitues
    printf("Pairs reconstitues: ");
    for (int i = 0; i < indexPairs; i++) {
        printf("%d ", octetPairs[i]);
    }
    printf("\n");

    printf("Impairs reconstitues: ");
    for (int i = 0; i < indexImpairs; i++) {
        printf("%d ", octetImpairs[i]);
    }
    printf("\n");

    // Affichage des representations binaires de pairs et impairs
    printf("Pairs: %s\n", pairs);
    printf("Impairs: %s\n", impairs);

    // Liberer la memoire allouee dynamiquement
    free(pairs);
    free(impairs);

    fclose(file); // Ferme le fichier


}





int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s nom_du_fichierEntree\n", argv[0]);
        return 1;
    }

    decodeFile(argv[1]);
    return 0;
}
