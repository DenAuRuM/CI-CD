#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef S21_CAT_H
#define S21_CAT_H

typedef struct s21_cat {
  bool numberNonBlank;
  bool endOftheStroke;
  bool numAllOutput;
  bool squeezeNeighbourStroke;
  bool displayTab;
  bool printNonPrintable;

} Flags;

Flags CatReadFlags(int argc, char *argv[]);

void CatFile(FILE *file, Flags flags, const char *table[static 256]);

void Cat(int argc, char *argv[], Flags flags, const char *table[static 256]);

void CatSetEndl(const char *table[static 256]);

void CatSetTab(const char *table[static 256]);

void CatSetTable(const char *table[static 256]);

void CatSetNonPrintable(const char *table[static 256]);

#endif /*S21_CAT_H*/