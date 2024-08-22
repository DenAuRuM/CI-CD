
#include "s21_grep.h"

int main(int argc, char *argv[]) {
  Flags flags = GrepReadFlags(argc, argv);
  Grep(argc, argv, flags);

  return 0;
}

Flags GrepReadFlags(int argc, char *argv[]) {
  int currentFlag = getopt_long(argc, argv, "eivcln", NULL, NULL);
  Flags flags = {0, 0, false, false, false, false};

  for (; currentFlag != -1;
       currentFlag = getopt_long(argc, argv, "eivcln", NULL, NULL)) {
    switch (currentFlag) {
      case 'e':
        flags.regex_flag |= REG_EXTENDED;
        break;
      case 'i':
        flags.regex_flag |= REG_ICASE;
        break;
      case 'v':
        flags.invert = true;
        break;
      case 'c':
        flags.count = true;
        break;
      case 'l':
        flags.filesMatch = true;
        break;
      case 'n':
        flags.numberLine = true;
    }
  }
  return flags;
}

void GrepCount(FILE *file, char const *filename, Flags flags, regex_t *preg,
               int argc) {
  (void)flags;
  (void)filename;
  int fl_name = argc - optind;
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  int count = 0;
  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      ++count;
    }
  }
  if (fl_name > 1) {
    printf("%s:%d\n", filename, count);
  } else {
    printf("%d\n", count);
  }

  free(line);
}

void PrintNumberCount(FILE *file, char const *filename, Flags flags,
                      regex_t *preg, int argc) {
  (void)flags;
  (void)filename;
  char *line = 0;
  size_t length = 0;
  int fl_name = argc - optind;
  regmatch_t match;
  int count = 0;
  while (getline(&line, &length, file) > 0) {
    count++;
    if (line[strlen(line) - 1] != '\n') {
      line[strlen(line)] = '\n';
    }
    if (!regexec(preg, line, 1, &match, 0)) {
      if (fl_name > 1) {
        printf("%s:%d:%s", filename, count, line);
      } else {
        printf("%d:%s", count, line);
      }
    }
    memset(line, 0, strlen(line));
  }

  free(line);
}
void MatchFile(FILE *file, char const *filename, Flags flags, regex_t *preg) {
  (void)flags;
  (void)filename;
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  bool yes_patt = false;

  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      yes_patt = true;
    }
  }
  if (yes_patt) {
    printf("%s\n", filename);
  }

  free(line);
}

void GrepFile(FILE *file, char const *filename, Flags flags, regex_t *preg,
              int argc) {
  (void)flags;
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  int fl_name = argc - optind;
  while (getline(&line, &length, file) > 0) {
    if (line[strlen(line) - 1] != '\n') {
      line[strlen(line)] = '\n';
    }
    if (flags.invert) {
      if (regexec(preg, line, 1, &match, 0)) {
        if (fl_name > 1) {
          printf("%s:%s", filename, line);
        } else {
          printf("%s", line);
        }
      }

    } else {
      if (!regexec(preg, line, 1, &match, 0)) {
        if (fl_name > 1) {
          printf("%s:%s", filename, line);
        } else {
          printf("%s", line);
        }
      }
    }
    memset(line, 0, strlen(line));
  }
  free(line);
}

void Grep(int argc, char *argv[], Flags flags) {
  char **pattern = &argv[optind];
  char **end = &argv[argc];
  optind++;
  regex_t preg_storage;
  regex_t *preg = &preg_storage;
  for (; pattern != end && pattern[0][0] == '-'; ++pattern)
    ;
  if (pattern == end) {
    fprintf(stderr, "There is no pattern\n");
    exit(1);
  }

  if (regcomp(preg, *pattern, flags.regex_flag)) {
    fprintf(stderr, "Failed to compile regex\n");
  }
  for (char **filename = pattern + 1; filename != end; ++filename) {
    if (**filename == '-') continue;
    FILE *file = fopen(*filename, "rb");
    if (errno) {
      fprintf(stderr, "%s", argv[0]);
      perror(*filename);
      continue;
    }
    if (flags.filesMatch) {
      MatchFile(file, *filename, flags, preg);
    }
    if (flags.numberLine) {
      PrintNumberCount(file, *filename, flags, preg, argc);
    }
    if (flags.count) {
      GrepCount(file, *filename, flags, preg, argc);
    } else
      GrepFile(file, *filename, flags, preg, argc);
    fclose(file);
  }
  regfree(preg);
}
