#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//la fonction qui encode en binaire ##################################################################
    void convertToBinary(int Data, char *binaryCode) {
        for (int i = 7; i >= 0; i--) {
            binaryCode[7 - i] = (( Data >> i) & 1) + '0';
        }
        binaryCode[8] = '\0'; // Terminateur de chaine
    }

//fonction qui va encoder le fichier entree et le mettre en fichier sortie ############################
    void encodeFile(char *fichierEntree,char *fichier_Sortie){

    //ouvrire le fichierEntree en lecture
        FILE *file = fopen(fichierEntree, "rb");

    //verifier si le fichier est vide ou bien il n existe pas  
        if (file == NULL || !file) {
            printf("Impossible d'ouvrir le fichier.\n");
            return;
        }
    //stocker les donnes lues a partir de fichierEntree
        char buffer[1024];
    //compter le nombre de caracteres dans le fichierEntree
        int numChar = 0;

    //nbr d'octets lus 
        size_t bytesRead = 0;
    //lire les donnes de fichierEntree par bloc
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            for (size_t i = 0; i < bytesRead; i++) {
                if (buffer[i] != '\n' && buffer[i] != '\r') {
                    numChar++;
                }
            }
        }
    //reinitialiser la position de lecture dans le fichierEntree 
        rewind(file);


    //Calcul du nombre de lignes supplementaires necessaires
        int nb_add_ligne = (5 - (numChar%5));

    //compter le nombre de lignes dans la matrix
        int nb_ligne = numChar + nb_add_ligne;

        //Alloue de la memoire pour un tableau de pointeurs de chaines de caracteres
        char **matrix = (char **)malloc(nb_ligne * sizeof(char *));
        for (int i = 0; i < nb_ligne; i++) {
            matrix[i] = (char *)malloc(9 * sizeof(char)); // 8 bits + 1 pour le terminateur
        }

        //suivre l'index de la ligne actuelle dans la matrice
        int indexLigne = 0;
        char add_line_1[9];
        char add_line[9];

        //stocker chaque caractere du fichier pendant la lecture
        int caractere;

        //copier le code binaire des caractaires dans la matrix et completer avec des lignes supplementaires #########
        //initialise des tableau pour creer les ligne a rajouter et les stocker temporairement
        if (nb_add_ligne != 0) {
            while (!feof(file)) {
                size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);

                for (size_t i = 0; i < bytesRead; i++) {
                    if (buffer[i] != '\n' && buffer[i] != '\r') {
                        char binaryCode[9];
                        convertToBinary(buffer[i], binaryCode);
                        strcpy(matrix[indexLigne], binaryCode);
                        indexLigne++;
                    }
                }
            }

            add_line_1[0] = '1';
            for (int i = 1; i < 8; i++) {
                add_line_1[i] = '0';
            }
            add_line_1[8] = '\0';

            for (int i = 0; i < 8; i++) {
                add_line[i] = '0';
            }
            add_line[8] = '\0';

            strcpy(matrix[indexLigne], add_line_1);
            indexLigne++;
            for (int i = 1; i < nb_add_ligne; i++) {
                strcpy(matrix[indexLigne], add_line);
                indexLigne++;
            }
        } else {
            while (!feof(file)) {
                size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);

                for (size_t i = 0; i < bytesRead; i++) {
                    if (buffer[i] != '\n' && buffer[i] != '\r') {
                        char binaryCode[9];
                        convertToBinary(buffer[i], binaryCode);
                        strcpy(matrix[indexLigne], binaryCode);
                        indexLigne++;
                    }
                }
            }
        }

        fclose(file);

    //aficher la matrix
        for (int i = 0; i < nb_ligne; i++) {
            printf("%s\n", matrix[i]);
        }

    //Alloue de la memoire pour deux tableaux pairs et impairs
        char *pairs = (char*)malloc(nb_ligne * 9 * sizeof(char)); // 9eme pour terminateur
        char *impairs = (char*)malloc(nb_ligne * 9 * sizeof(char));
        
    //initialiser des indices pour les tableau pairs impairs
        int indexPairs = 0;
        int indexImpairs = 0;
        int cptPairs = 0;
        int cptImpairs = 0;

    //copier les elements dans les tableaux
        for (int i = 0; i < nb_ligne; i++) {
            for (int j = 0; j < 9; j++) {
                char Box = matrix[i][j];
                if (j % 2 == 0){
                    // Ajout de 3 zeros avant chaque serie de 5 caracteres
                    if (cptPairs == 0 || cptPairs % 5 == 0) {
                        for (int k = 0; k < 3; k++) {
                            pairs[indexPairs++] = '0';
                        }
                    }
                    pairs[indexPairs] = Box;
                    indexPairs++;
                    cptPairs++;
                } else {
                    // Ajout de 3 zeros avant chaque serie de 5 caracteres
                    if (cptImpairs == 0 || cptImpairs % 5 == 0) {
                        for (int k = 0; k < 3; k++) {
                            impairs[indexImpairs++] = '0';
                        }
                    }
                    impairs[indexImpairs] = Box;
                    indexImpairs++;
                    cptImpairs++;
                }
            }
        }

    // Affichage des tableaux pairs et impairs
        printf("Pairs: ");
        for (int i = 0; i < indexPairs; i++) {
            printf("%c ", pairs[i]);
        }
        printf("\n");

        printf("Impairs: ");
        for (int i = 0; i < indexImpairs; i++) {
            printf("%c ", impairs[i]);
        }
        printf("\n");

    // Liberation de la memoire allouee
        for (int i = 0; i < numChar; i++) {
            free(matrix[i]);
        }
            free(matrix);

    // Ouvrir un fichier de sortie
        FILE *outputFile = fopen(fichier_Sortie, "wb");
            if (outputFile == NULL || !outputFile) {
                printf("Impossible d'ouvrir le fichier de sortie.\n");
            
            }

    // Fonction pour convertir un nombre binaire en decimal
        int binaryToDecimal(int binary) {
            int decimal = 0;
            int base = 1;
            int remainder;

            while (binary > 0) {
                remainder = binary % 10;
                decimal = decimal + remainder * base;
                base = base * 2;
                binary = binary / 10;
            }

            return decimal;
        }

    // Convertir les 8 bits de pairs et impairs en octets puis en deecimal
        for (int i = 0; i < indexPairs; i += 8) {
            unsigned char octetPairs = 0;
            unsigned char octetImpairs = 0;

            // Construction de l'octet a partir du tableau pairs
            for (int j = i; j < i + 8; j++) {
                octetPairs <<= 1;
                octetPairs |= (pairs[j] - '0');
            }

            // Construction de l'octet a partir du tableau impairs
            for (int j = i; j < i + 8; j++) {
                octetImpairs <<= 1;
                octetImpairs |= (impairs[j] - '0');
            }

            // ecrire les octets en decimal dans le fichier de sortie
            fprintf(outputFile, "%d\n", binaryToDecimal(octetPairs));
            fprintf(outputFile, "%d\n", binaryToDecimal(octetImpairs));
        }

    // Fermer le fichier de sortie
        fclose(outputFile);

        free(impairs);
        free(pairs);
    }

//###################################    main    ##########################################
    int main(int argc, char *argv[]) {
        if (argc != 3) {
            printf("Usage: %s nom_du_fichierEntre nom_du_fichierEntre\n", argv[0]);
            return 1;
        }
        encodeFile(argv[1],argv[2]);
        return 0;
    }
