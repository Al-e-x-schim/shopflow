#ifndef COMMANDE_H
#define COMMANDE_H

#include "types.h"

void calculerCommande(Commande *cmd);

void creerCommande(Boutique *b);

void ajouterLigneCommande(Commande *cmd, Boutique *b);
void supprimerLigneCommande(Commande *cmd, Boutique *b);

void changerStatutCommande(Commande *cmd);
void annulerCommande(Commande *cmd, Boutique *b);


void afficherDetailCommande(const Commande *cmd);

#endif