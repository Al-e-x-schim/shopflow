#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statistique.h"

float chiffreAffairesTotalHT(Boutique *b) {
    float t = 0;
    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            if (strcmp(b->clients[i].commandes[j].statut, "ANNULEE") != 0) {
                t += b->clients[i].commandes[j].montantHT;
            }
        }
    }
    return t;
}

float chiffreAffairesTotalTTC(Boutique *b) {
    float t = 0;
    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            if (strcmp(b->clients[i].commandes[j].statut, "ANNULEE") != 0) {
                t += b->clients[i].commandes[j].montantTTC;
            }
        }
    }
    return t;
}

typedef struct { char ref[12]; char designation[60]; int qte; } PalmaresProd;

void palmaresProduits(Boutique *b) {
    PalmaresProd tab[100]; int n = 0;
    for (int i = 0; i < b->nbProduits; i++) {
        if (b->produits[i].actif) { 
            strcpy(tab[n].ref, b->produits[i].reference); 
            strcpy(tab[n].designation, b->produits[i].designation); 
            tab[n].qte = 0; n++; 
        }
    }
    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            Commande *c = &b->clients[i].commandes[j]; 
            if (strcmp(c->statut, "ANNULEE") == 0) continue;
            for (int k = 0; k < c->nbLignes; k++) {
                for (int l = 0; l < n; l++) {
                    if (strcmp(tab[l].ref, c->lignes[k].refProduit) == 0) {
                        tab[l].qte += c->lignes[k].quantite;
                    }
                }
            }
        }
    }
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (tab[j].qte < tab[j + 1].qte) { 
                PalmaresProd tmp = tab[j]; tab[j] = tab[j + 1]; tab[j + 1] = tmp; 
            }
        }
    }
    printf("\n=== TOP 5 DES PRODUITS LES PLUS VENDUS ===\n");
    for (int i = 0; i < n && i < 5; i++) {
        printf("%d. %-25s (Ref: %s) : %d vendus\n", i + 1, tab[i].designation, tab[i].ref, tab[i].qte);
    }
}

typedef struct { int id; char nom[50]; char prenom[50]; float ca; } PalmaresCli;

void palmaresClients(Boutique *b) {
    PalmaresCli tab[100]; int n = 0;
    for (int i = 0; i < b->nbClients; i++) {
        if (b->clients[i].actif) {
            tab[n].id = b->clients[i].idClient; 
            strcpy(tab[n].nom, b->clients[i].nom); 
            strcpy(tab[n].prenom, b->clients[i].prenom); 
            tab[n].ca = 0;
            for (int j = 0; j < b->clients[i].nbCommandes; j++) {
                if (strcmp(b->clients[i].commandes[j].statut, "ANNULEE") != 0) {
                    tab[n].ca += b->clients[i].commandes[j].montantTTC;
                }
            }
            n++;
        }
    }
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (tab[j].ca < tab[j + 1].ca) { 
                PalmaresCli tmp = tab[j]; tab[j] = tab[j + 1]; tab[j + 1] = tmp; 
            }
        }
    }
    printf("\n=== PALMARES DES CLIENTS (Par CA genere) ===\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s %s (ID: %d) : %.2f EUR\n", i + 1, tab[i].nom, tab[i].prenom, tab[i].id, tab[i].ca);
    }
}