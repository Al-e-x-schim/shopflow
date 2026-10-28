#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commande.h"
#include "types.h"

void calculerCommande(Commande *cmd) {
    cmd->montantBrutHT = 0.0f;
    
    for (int i = 0; i < cmd->nbLignes; i++) {
        cmd->lignes[i].sousTotalHT = cmd->lignes[i].quantite * cmd->lignes[i].prixUnitaireHT;
        cmd->montantBrutHT += cmd->lignes[i].sousTotalHT;
    }

    if (cmd->montantBrutHT >= 600.0f) {
        cmd->tauxRemise = 15.0f;
    } else if (cmd->montantBrutHT >= 300.0f) {
        cmd->tauxRemise = 10.0f;
    } else if (cmd->montantBrutHT >= 100.0f) {
        cmd->tauxRemise = 5.0f;
    } else {
        cmd->tauxRemise = 0.0f;
    }

    cmd->montantRemise = cmd->montantBrutHT * (cmd->tauxRemise / 100.0f);
    cmd->montantHT = cmd->montantBrutHT - cmd->montantRemise;

    cmd->montantTVA = cmd->montantHT * TVA;

    if (cmd->montantBrutHT >= SEUIL_PORT_OFFERT) {
        cmd->fraisPort = 0.0f;
    } else {
        cmd->fraisPort = FRAIS_PORT;
    }

    cmd->montantTTC = cmd->montantHT + cmd->montantTVA + cmd->fraisPort;
}

void creerCommande(Boutique *b) {
    printf("\n--- CRÉATION D'UNE NOUVELLE COMMANDE ---\n");

    if (b->nbClients == 0) {
        printf("[X] Erreur : Aucun client enregistré. Créez d'abord un client.\n");
        return;
    }

    int idClientRecherche = saisirEntier("Entrez l'identifiant (ID) du client : ");
    
    Client *clientTrouve = NULL;
    for (int i = 0; i < b->nbClients; i++) {
        if (b->clients[i].idClient == idClientRecherche && b->clients[i].actif == 1) {
            clientTrouve = &b->clients[i];
            break;
        }
    }

    if (!clientTrouve) {
        printf("[X] Client introuvable ou inactif.\n");
        return;
    }

    if (clientTrouve->nbCommandes >= clientTrouve->capaciteCommandes) {
        clientTrouve->capaciteCommandes = (clientTrouve->capaciteCommandes == 0) ? 2 : clientTrouve->capaciteCommandes * 2;
        Commande *temp = realloc(clientTrouve->commandes, clientTrouve->capaciteCommandes * sizeof(Commande));
        if (!temp) {
            printf("[X] Erreur d'allocation mémoire pour les commandes du client.\n");
            exit(1);
        }
        clientTrouve->commandes = temp;
    }

    Commande nouvelleCmd;
    nouvelleCmd.numero = b->numeroSuivant++; 
    nouvelleCmd.idClient = clientTrouve->idClient;

    printf("Date de la commande :\n");
    nouvelleCmd.dateCommande.jour = saisirEntier("  Jour (1-31) : ");
    nouvelleCmd.dateCommande.mois = saisirEntier("  Mois (1-12) : ");
    nouvelleCmd.dateCommande.annee = saisirEntier("  Année (2000-2100) : ");

    strcpy(nouvelleCmd.statut, "EN_COURS");

    nouvelleCmd.capaciteLignes = 2;
    nouvelleCmd.nbLignes = 0;
    nouvelleCmd.lignes = (LigneCommande *)malloc(nouvelleCmd.capaciteLignes * sizeof(LigneCommande));
    if (!nouvelleCmd.lignes) {
        printf("[X] Erreur d'allocation mémoire pour les lignes de commande.\n");
        exit(1);
    }

    nouvelleCmd.montantBrutHT = 0.0f;
    nouvelleCmd.tauxRemise = 0.0f;
    nouvelleCmd.montantRemise = 0.0f;
    nouvelleCmd.montantHT = 0.0f;
    nouvelleCmd.montantTVA = 0.0f;
    nouvelleCmd.montantTTC = 0.0f;
    nouvelleCmd.fraisPort = FRAIS_PORT;

    nouvelleCmd.adresseLivraison = clientTrouve->adresse;

    clientTrouve->commandes[clientTrouve->nbCommandes] = nouvelleCmd;
    clientTrouve->nbCommandes++;

    printf("[OK] Commande n°%d créée avec succès pour le client %s %s !\n", 
           nouvelleCmd.numero, clientTrouve->prenom, clientTrouve->nom);
