#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "utils.h"

void initialiserClients(Boutique *b) {
    b->capaciteClients = CAPACITE_INITIALE; 
    b->nbClients = 0;
    b->clients = (Client*)malloc(b->capaciteClients * sizeof(Client));
}

Client* trouverClientParId(Boutique *b, int id) {
    for (int i = 0; i < b->nbClients; i++) {
        if (b->clients[i].idClient == id) return &b->clients[i];
    }
    return NULL;
}

int ajouterClient(Boutique *b, Client c) {
    if (b->nbClients == b->capaciteClients) { 
        b->capaciteClients *= 2; 
        b->clients = (Client*)realloc(b->clients, b->capaciteClients * sizeof(Client)); 
    }
    b->clients[b->nbClients++] = c; 
    return 1;
}

void afficherListeClients(Boutique *b) {
    printf("\n=== LISTE DES CLIENTS ===\n");
    for (int i = 0; i < b->nbClients; i++) {
        if (b->clients[i].actif) {
            printf("ID: %d | %-10s %-10s | %s\n", 
                   b->clients[i].idClient, b->clients[i].nom, 
                   b->clients[i].prenom, b->clients[i].email);
        }
    }
}

void afficherFicheClient(Boutique *b) {
    int id = saisirEntier("ID du client : ");
    Client *c = trouverClientParId(b, id);
    if (!c) { printf("Client introuvable.\n"); return; }
    
    printf("\n=== FICHE CLIENT ===\n");
    printf("Nom : %s %s\n", c->nom, c->prenom);
    printf("Email : %s\n", c->email);
    printf("Points fidelite : %d\n", c->pointsFidelite);
    printf("Historique des commandes (%d) :\n", c->nbCommandes);
    for (int i = 0; i < c->nbCommandes; i++) {
        printf("  #%d | Statut: %s | TTC: %.2f EUR\n", 
               c->commandes[i].numero, c->commandes[i].statut, c->commandes[i].montantTTC);
    }
}