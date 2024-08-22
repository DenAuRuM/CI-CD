#include "s21_cat.h"

Flags CatReadFlags(int argc, char *argv[]) {
  struct option longOptions[] = {
      {"number-nonblank", 0, NULL, 'b'},
      {"number", 0, NULL, 'n'},
      {"squeeze-blank", 0, NULL, 's'},
      {NULL, 0, NULL, 0}  // The end of the array
  };
  int currentFlag = getopt_long(argc, argv, "+benstuvTE", longOptions, NULL);
  Flags flags = {
      false, false, false, false,
      false, false};  // Мы поймем, что флаг не был установлен и остановится
  for (; currentFlag != -1;
       currentFlag = getopt_long(argc, argv, "+benstuvTE", longOptions, NULL)) {
    switch (currentFlag) {
      case 'b':
        flags.numberNonBlank = true;
        break;
      case 'e':
        flags.printNonPrintable = true;
        flags.endOftheStroke = true;
        break;
      case 'E':
        flags.endOftheStroke = true;
        break;
      case 'v':
        flags.printNonPrintable = true;
        break;
      case 'n':
        flags.numAllOutput = true;
        break;
      case 's':
        flags.squeezeNeighbourStroke = true;
        break;
      case 't':
        flags.printNonPrintable = true;
        flags.displayTab = true;
        break;
      case 'T':
        flags.displayTab = true;
        break;
    }
  }
  return flags;
}

void CatFile(FILE *file, Flags flags, const char *table[static 256]) {
  int c = 0;
  int last;
  bool squeeze = false;
  int symbols = 0;
  last = '\n';
  while (fread(&c, 1, 1, file) > 0) {
    if (last == '\n') {
      if (flags.squeezeNeighbourStroke && c == '\n') {
        if (squeeze) continue;
        squeeze = true;
      } else
        squeeze = false;
      if (flags.numberNonBlank) {
        if (c != '\n') printf("%6i\t", ++symbols);
      } else if (flags.numAllOutput) {
        printf("%6i\t", ++symbols);
      }
    }
    if (!*table[c])
      printf("%c", '\0');
    else
      printf("%s", table[c]);
    last = c;
  }
}

void Cat(int argc, char *argv[], Flags flags, const char *table[static 256]) {
  for (char **filename = &argv[1], **end = &argv[argc]; filename != end;
       ++filename) {
    if (**filename == '-') continue;
    FILE *file = fopen(*filename, "rb");
    if (errno) {
      fprintf(stderr, "%s", argv[0]);
      perror(*filename);
      continue;
    }
    CatFile(file, flags, table);
    fclose(file);
  }
}

void CatSetEndl(const char *table[static 256]) { table['\n'] = "$\n"; }

void CatSetTab(const char *table[static 256]) { table['\t'] = "^I"; }

int main(int argc, char *argv[]) {
  Flags flags = CatReadFlags(argc, argv);
  const char *table[256];

  CatSetTable(table);
  if (flags.endOftheStroke) CatSetEndl(table);
  if (flags.displayTab) CatSetTab(table);
  if (flags.printNonPrintable) CatSetNonPrintable(table);

  Cat(argc, argv, flags, table);
  return 0;
}
