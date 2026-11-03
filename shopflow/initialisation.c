#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "initialisation.h"
#include "produit.h"
#include "client.h"
#include "commande.h"

void initialiserBoutique(Boutique *b) {
    strcpy(b->nom, "ShopFlow"); 
    initialiserProduits(b); 
    initialiserClients(b); 
    b->numeroSuivant = 1001; 
    b->idClientSuivant = 1;
}

static void ajouterProduitDemo(Boutique *b, const char *ref, const char *des, const char *cat, float prix, int stock, int seuil) {
    Produit p; memset(&p, 0, sizeof(Produit)); 
    strcpy(p.reference, ref); strcpy(p.designation, des); strcpy(p.categorie, cat);
    p.prixUnitaireHT = prix; p.quantiteStock = stock; p.seuilAlerte = seuil; p.actif = 1; 
    ajouterProduit(b, p);
}

static void ajouterClientDemo(Boutique *b, const char *nom, const char *prenom, const char *email, const char *tel, const char *rue, const char *ville, const char *cp) {
    Client c; memset(&c, 0, sizeof(Client)); 
    c.idClient = b->idClientSuivant++; 
    strcpy(c.nom, nom); strcpy(c.prenom, prenom); strcpy(c.email, email); strcpy(c.telephone, tel);
    strcpy(c.adresse.rue, rue); strcpy(c.adresse.ville, ville); strcpy(c.adresse.codePostal, cp); strcpy(c.adresse.pays, "France");
    c.actif = 1; 
    ajouterClient(b, c);
}

void chargerDonneesDemo(Boutique *b) {
    ajouterProduitDemo(b, "PRD-0001", "Clavier mecanique", "Informatique", 79.90f, 42, 10);
    ajouterProduitDemo(b, "PRD-0002", "Souris sans fil", "Informatique", 29.90f, 8, 10);
    ajouterProduitDemo(b, "PRD-0003", "Ecran 27 pouces", "Informatique", 249.00f, 15, 5);
    ajouterProduitDemo(b, "PRD-0004", "Cable HDMI 2m", "Accessoires", 12.50f, 3, 5);
    ajouterProduitDemo(b, "PRD-0005", "Casque audio", "Audio", 89.00f, 25, 8);
    ajouterProduitDemo(b, "PRD-0006", "Enceinte Bluetooth", "Audio", 59.90f, 18, 6);
    ajouterProduitDemo(b, "PRD-0007", "Webcam HD", "Informatique", 49.90f, 30, 10);
    ajouterProduitDemo(b, "PRD-0008", "Hub USB 4 ports", "Accessoires", 19.90f, 12, 5);

    ajouterClientDemo(b, "Bernard", "Julie", "j.bernard@mail.fr", "0612345678", "12 rue de la Paix", "Paris", "75002");
    ajouterClientDemo(b, "Martin", "Lucas", "l.martin@mail.fr", "0623456789", "5 avenue Victor Hugo", "Lyon", "69002");
    ajouterClientDemo(b, "Petit", "Emma", "e.petit@mail.fr", "0634567890", "8 rue Nationale", "Lille", "59000");
    ajouterClientDemo(b, "Durand", "Hugo", "h.durand@mail.fr", "0645678901", "3 place Bellecour", "Lyon", "69002");
    ajouterClientDemo(b, "Leroy", "Chloe", "c.leroy@mail.fr", "0656789012", "20 rue Sainte-Catherine", "Bordeaux", "33000");
}

void libererBoutique(Boutique *b) {
    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            free(b->clients[i].commandes[j].lignes);
        }
        free(b->clients[i].commandes);
    }
    free(b->clients);
    free(b->produits);
}