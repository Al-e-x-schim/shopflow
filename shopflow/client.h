#ifndef CLIENT_H
#define CLIENT_H
#include "types.h"

void initialiserClients(Boutique *b);
Client* trouverClientParId(Boutique *b, int id);
int ajouterClient(Boutique *b, Client c);
void afficherListeClients(Boutique *b);
void afficherFicheClient(Boutique *b);

#endif