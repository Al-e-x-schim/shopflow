#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produit.h"
#include "utils.h"

void initialiserProduits(Boutique *b) {
    b->capaciteProduits = CAPACITE_INITIALE; 
    b->nbProduits = 0;
    b->produits = (Produit*)malloc(b->capaciteProduits * sizeof(Produit));
}

int ajouterProduit(Boutique *b, Produit p) {
    if (b->nbProduits == b->capaciteProduits) { 
        b->capaciteProduits *= 2; 
        b->produits = (Produit*)realloc(b->produits, b->capaciteProduits * sizeof(Produit)); 
    }
    b->produits[b->nbProduits++] = p; 
    return 1;
}

Produit* trouverProduitParRef(Boutique *b, const char *ref) {
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, ref) == 0) return &b->produits[i];
    }
    return NULL;
}

void afficherCatalogue(Boutique *b) {
    printf("\n=== CATALOGUE ===\n");
    for (int i = 0; i < b->nbProduits; i++) {
        if (b->produits[i].actif) {
            printf("%-10s | %-25s | %6.2f EUR | Stock: %d\n", 
                   b->produits[i].reference, b->produits[i].designation, 
                   b->produits[i].prixUnitaireHT, b->produits[i].quantiteStock);
        }
    }
}

void alerterStock(Boutique *b) {
    printf("\n=== ALERTES DE STOCK ===\n");
    int trouve = 0;
    for (int i = 0; i < b->nbProduits; i++) {
        if (b->produits[i].actif && b->produits[i].quantiteStock <= b->produits[i].seuilAlerte) {
            printf("[!] %-10s | %-25s | Stock: %d (Seuil: %d)\n", 
                   b->produits[i].reference, b->produits[i].designation, 
                   b->produits[i].quantiteStock, b->produits[i].seuilAlerte);
            trouve = 1;
        }
    }
    if (!trouve) printf("Aucune alerte de stock.\n");
}