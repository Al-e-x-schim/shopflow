#ifndef UTILS_H
#define UTILS_H
#include "types.h"

void viderBuffer(void);
int saisirEntier(const char *message);
float saisirFloat(const char *message);
void saisirChaine(const char *message, char *dest, int taille);
void afficherDate(Date d);
void afficherAdresse(Adresse a);

#endif