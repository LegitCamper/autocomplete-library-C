#include "auto_correct.h"

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void print_suggestions(char *misspelled_word) {
  static char suggestions[MAX_SUGGESTIONS][MAX_LENGTH + 1];

  printf("Misspelled word: %s\n", misspelled_word);
  int num_suggestions = suggest(misspelled_word, suggestions);
  if (num_suggestions == 0) {
    puts("Spelled correctly\n");
  } else {
    puts("Corrections:");
    for (int i = 0; i < num_suggestions; ++i) {
      if (i + 1 == num_suggestions) {
        printf("%s\n\n", suggestions[i]);
      } else {
        printf("%s, ", suggestions[i]);
      }
    }
  }
}

int main() {
  print_suggestions("catt");
  print_suggestions("theme");
  print_suggestions("whate");
  print_suggestions("doge");
  print_suggestions("dodgee");
  print_suggestions("bannana");
  print_suggestions("wherre");

  return 0;
}
