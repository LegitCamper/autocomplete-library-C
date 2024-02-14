#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>

void print_words(struct TrieNode *root, char *word) {
  struct TrieNode temp = *root;
  for (int i = 0; i < N; ++i) {
    if (temp.children[i] != 0) {
      char *new_word = malloc(strlen(word) + 2);
      strcpy(new_word, word);
      new_word[strlen(word)] = 'a' + i;
      new_word[strlen(word) + 1] = '\0';
      print_words(temp.children[i], new_word);

      if (temp.children[i]->is_leaf == 1) {
        printf("%s\n", word);
      }
    }
  }
}

bool check(const char *word) {
  int idx;
  struct TrieNode *child = &TRIE_ROOT;

  for (int i = 0, l = strlen(word); i < l; i++) {
    idx = tolower(word[i]) - 'a';

    child = child->children[idx];
    if (child == NULL) {
      return false;
    }
  }
  return child->is_leaf;
}


#define MAX_SUGGESTIONS 10
#define MAX_LENGTH 20
#define BEST_DISTANCE 3

int min(int a, int b, int c) {
  return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int levenshtein_distance(const char *s, const char *t) {
  int n = strlen(s);
  int m = strlen(t);

  if (n == 0)
    return m;
  if (m == 0)
    return n;

  int dp[MAX_LENGTH + 1][MAX_LENGTH + 1];
  for (int i = 0; i <= n; i++)
    dp[i][0] = i;
  for (int j = 0; j <= m; j++)
    dp[0][j] = j;

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= m; j++) {
      int cost = (s[i - 1] == t[j - 1]) ? 0 : 1;
      dp[i][j] =
          min(dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost);
    }
  }
  return dp[n][m];
}

void calculate_levenshtein_distance(
    char *mispelled_word, char *dict_word, int *num_suggestions,
    char suggestions[MAX_SUGGESTIONS][MAX_LENGTH + 1], int *min_distance) {
  int distance = levenshtein_distance(mispelled_word, dict_word);
  if (distance < *min_distance && distance < BEST_DISTANCE) {
    *num_suggestions = 0;
    strcpy(suggestions[*num_suggestions], dict_word);
    (*num_suggestions)++;
    *min_distance = distance;
  } else if (distance == *min_distance && distance < BEST_DISTANCE) {
    strcpy(suggestions[*num_suggestions], dict_word);
    (*num_suggestions)++;
  }
}

void read_words(struct TrieNode *root, char *word, char *typed_word,
                int *num_suggestions, int *min_distance,
                char suggestions[MAX_SUGGESTIONS][MAX_LENGTH + 1]) {
  struct TrieNode temp = *root;

  if (temp.is_leaf == 1) {
    calculate_levenshtein_distance(typed_word, word, num_suggestions,
                                   suggestions, min_distance);
  }

  for (int i = 0; i < N; ++i) {
    if (*num_suggestions == MAX_SUGGESTIONS) {
      return;
    }

    if (temp.children[i] != NULL) {
      char *new_word = malloc(strlen(word) + 2);
      strcpy(new_word, word);
      new_word[strlen(word)] = 'a' + i;
      new_word[strlen(word) + 1] = '\0';
      read_words(temp.children[i], new_word, typed_word, num_suggestions,
                 min_distance, suggestions);
      free(new_word);
    }
  }
}

int suggest(const char *word,
             char suggestions[MAX_SUGGESTIONS][MAX_LENGTH + 1]) {
  if (check(word) == true) {
    // Already correct
    return 0;
  }
  int num_suggestions = 0;
  int min_distance = INT_MAX;

  // Read words from trie and calculate Levenshtein distance
  read_words(&TRIE_ROOT, "", (char *)word, &num_suggestions, &min_distance,
             suggestions);
  return num_suggestions;
}

