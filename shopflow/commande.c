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

void ajouterLigneCommande(Commande *cmd, Boutique *b) {
    if (strcmp(cmd->statut, "EN_COURS") != 0) {
        printf("[X] Erreur : Modification interdite (la commande n'est plus au statut EN_COURS).\n");
        return;
    }

    char ref[12];
    printf("Entrez la référence du produit à commander : ");
    if (scanf("%11s", ref) != 1) return;
    viderBuffer();

    Produit *p = NULL;
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, ref) == 0 && b->produits[i].actif == 1) {
            p = &b->produits[i];
            break;
        }
    }

    if (!p) {
        printf("[X] Produit introuvable ou inactif au catalogue.\n");
        return;
    }

    int qte = saisirEntier("Entrez la quantité souhaitée : ");
    if (qte <= 0 || qte > p->quantiteStock) {
        printf("[X] Quantité invalide ou stock insuffisant (Stock disponible : %d).\n", p->quantiteStock);
        return;
    }

    int indexExistant = -1;
    for (int i = 0; i < cmd->nbLignes; i++) {
        if (strcmp(cmd->lignes[i].refProduit, ref) == 0) {
            indexExistant = i;
            break;
        }
    }

    if (indexExistant != -1) {

        if (cmd->lignes[indexExistant].quantite + qte > p->quantiteStock) {
            printf("[X] Stock insuffisant pour le cumul de cette quantité.\n");
            return;
        }
        cmd->lignes[indexExistant].quantite += qte;
    } else {

        if (cmd->nbLignes >= cmd->capaciteLignes) {
            cmd->capaciteLignes *= 2;
            LigneCommande *temp = realloc(cmd->lignes, cmd->capaciteLignes * sizeof(LigneCommande));
            if (!temp) {
                printf("[X] Erreur d'allocation mémoire pour les lignes.\n");
                exit(1);
            }
            cmd->lignes = temp;
        }

        strcpy(cmd->lignes[cmd->nbLignes].refProduit, p->reference);
        strcpy(cmd->lignes[cmd->nbLignes].designation, p->designation);
        cmd->lignes[cmd->nbLignes].quantite = qte;
        cmd->lignes[cmd->nbLignes].prixUnitaireHT = p->prixUnitaireHT;
        cmd->nbLignes++;
    }

    p->quantiteStock -= qte;

    calculerCommande(cmd);
    printf("[OK] Ligne ajoutée avec succès et stock mis à jour !\n");
}       

void supprimerLigneCommande(Commande *cmd, Boutique *b) {
    if (strcmp(cmd->statut, "EN_COURS") != 0) {
        printf("[X] Erreur : Suppression interdite (la commande n'est plus au statut EN_COURS).\n");
        return;
    }

    if (cmd->nbLignes == 0) {
        printf("[X] La commande ne contient aucune ligne.\n");
        return;
    }

    char ref[12];
    printf("Entrez la référence du produit à supprimer de la commande : ");
    if (scanf("%11s", ref) != 1) return;
    viderBuffer();

    int indexLigne = -1;
    for (int i = 0; i < cmd->nbLignes; i++) {
        if (strcmp(cmd->lignes[i].refProduit, ref) == 0) {
            indexLigne = i;
            break;
        }
    }

    if (indexLigne == -1) {
        printf("[X] Produit introuvable dans cette commande.\n");
        return;
    }

    int qteARetourner = cmd->lignes[indexLigne].quantite;

    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, ref) == 0) {
            b->produits[i].quantiteStock += qteARetourner;
            break;
        }
    }

    for (int i = indexLigne; i < cmd->nbLignes - 1; i++) {
        cmd->lignes[i] = cmd->lignes[i + 1];
    }
    cmd->nbLignes--;

    calculerCommande(cmd);
    printf("[OK] Ligne supprimée avec succès et stock réintégré !\n");
}
void changerStatutCommande(Commande *cmd) {
    if (strcmp(cmd->statut, "EN_COURS") == 0) {
        strcpy(cmd->statut, "EXPEDIEE");
        printf("[OK] Statut mis à jour : la commande est désormais EXPEDIEE.\n");
    } else if (strcmp(cmd->statut, "EXPEDIEE") == 0) {
        strcpy(cmd->statut, "LIVREE");
        printf("[OK] Statut mis à jour : la commande est désormais LIVREE.\n");
    } else {
        printf("[X] Impossible de faire progresser cette commande (statut actuel : %s).\n", cmd->statut);
    }
}

void annulerCommande(Commande *cmd, Boutique *b) {
    if (strcmp(cmd->statut, "EN_COURS") != 0) {
        printf("[X] Annulation impossible : la commande n'est plus au statut EN_COURS.\n");
        return;
    }

    for (int i = 0; i < cmd->nbLignes; i++) {
        for (int j = 0; j < b->nbProduits; j++) {
            if (strcmp(b->produits[j].reference, cmd->lignes[i].refProduit) == 0) {
                b->produits[j].quantiteStock += cmd->lignes[i].quantite;
                break;
            }
        }
    }

    strcpy(cmd->statut, "ANNULEE");
    printf("[OK] Commande annulée avec succès. Tous les stocks ont été réintégrés.\n");
}
void afficherDetailCommande(const Commande *cmd) {
    printf("\n========================================\n");
    printf("         FACTURE / COMMANDE N°%d          \n", cmd->numero);
    printf("========================================\n");
    printf("ID Client : %d\n", cmd->idClient);
    printf("Date      : %02d/%02d/%d\n", cmd->dateCommande.jour, cmd->dateCommande.mois, cmd->dateCommande.annee);
    printf("Statut    : %s\n", cmd->statut);
    printf("----------------------------------------\n");
    printf("Lignes de commande :\n");
    
    if (cmd->nbLignes == 0) {
        printf("  (Aucun produit dans cette commande)\n");
    } else {
        for (int i = 0; i < cmd->nbLignes; i++) {
            printf("  - [%s] %s | Qté : %d | P.U. HT : %.2f € | Sous-total : %.2f €\n",
                   cmd->lignes[i].refProduit,
                   cmd->lignes[i].designation,
                   cmd->lignes[i].quantite,
                   cmd->lignes[i].prixUnitaireHT,
                   cmd->lignes[i].sousTotalHT);
        }
    }
    
    printf("----------------------------------------\n");
    printf("Montant Brut HT : %.2f €\n", cmd->montantBrutHT);
    printf("Remise (%.0f%%)    : -%.2f €\n", cmd->tauxRemise, cmd->montantRemise);
    printf("Montant HT      : %.2f €\n", cmd->montantHT);
    printf("TVA (20%)       : %.2f €\n", cmd->montantTVA);
    printf("Frais de port   : %.2f €\n", cmd->fraisPort);
    printf("========================================\n");
    printf("MONTANT TTC     : %.2f €\n", cmd->montantTTC);
    printf("========================================\n");
}