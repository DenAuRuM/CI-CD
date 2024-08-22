
#define _GNU_SOURCE

#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct {
  char *pattern;

  int regex_flag;
  bool invert;
  bool count;
  bool filesMatch;
  bool numberLine;
} Flags;

Flags GrepReadFlags(int argc, char *argv[]);
void GrepCount(FILE *file, char const *filename, Flags flags, regex_t *preg,
               int argc);
void PrintNumberCount(FILE *file, char const *filename, Flags flags,
                      regex_t *preg, int argc);
void MatchFile(FILE *file, char const *filename, Flags flags, regex_t *preg);
void GrepFile(FILE *file, char const *filename, Flags flags, regex_t *preg,
              int argc);
void Grep(int argc, char *argv[], Flags flags);
