#ifndef PRODUIT_H
#define PRODUIT_H
#include "types.h"

void initialiserProduits(Boutique *b);
int ajouterProduit(Boutique *b, Produit p);
Produit* trouverProduitParRef(Boutique *b, const char *ref);
void afficherCatalogue(Boutique *b);
void alerterStock(Boutique *b);

#endif