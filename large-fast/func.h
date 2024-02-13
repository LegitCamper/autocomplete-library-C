#include <string.h>
#include <stdio.h>

void print_words(struct TrieNode *root, char *word) {
  struct TrieNode temp = *root;
  for (int i = 0; i < N; ++i) {
    if (temp.children[i] != 0) {
      char *new_word = malloc(strlen(word) + 2);
      strcpy(new_word, word);
      strcat(new_word, &temp.children[i]->data);
      print_words(temp.children[i], new_word);

      if (temp.children[i]->is_leaf == 1) {
        printf("%s\n", word);
      }
    }
  }
}


