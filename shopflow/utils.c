#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void viderBuffer(void) { 
    int c; 
    while ((c = getchar()) != '\n' && c != EOF); 
}

int saisirEntier(const char *message) {
    int valeur; char buffer[100]; int ok = 0;
    while (!ok) { 
        printf("%s", message); 
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) { 
            if (sscanf(buffer, "%d", &valeur) == 1) ok = 1; 
            else printf("Erreur : veuillez saisir un entier valide.\n"); 
        } 
    }
    return valeur;
}

float saisirFloat(const char *message) {
    float valeur; char buffer[100]; int ok = 0;
    while (!ok) { 
        printf("%s", message); 
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) { 
            if (sscanf(buffer, "%f", &valeur) == 1) ok = 1; 
            else printf("Erreur : veuillez saisir un nombre valide.\n"); 
        } 
    }
    return valeur;
}

void saisirChaine(const char *message, char *dest, int taille) {
    int ok = 0;
    while (!ok) {
        printf("%s", message);
        if (fgets(dest, taille, stdin) != NULL) {
            size_t len = strlen(dest);
            if (len > 0 && dest[len - 1] == '\n') dest[len - 1] = '\0'; 
            else viderBuffer();
            if (strlen(dest) > 0) ok = 1; 
            else printf("Erreur : la saisie ne peut pas etre vide.\n");
        }
    }
}

void afficherDate(Date d) { printf("%02d/%02d/%04d", d.jour, d.mois, d.annee); }
void afficherAdresse(Adresse a) { printf("%s, %s %s, %s", a.rue, a.codePostal, a.ville, a.pays); }