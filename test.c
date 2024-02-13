#include "autocorrect.h"

#include <string.h>

search;

int main() {
  char *arr[] = {};
  char **array = words_array(&TRIE_ROOT, "", arr, 0);
  int array_len = sizeof(array);
  printf("%i\n", array_len);
  for (int i = 0; i < array_len; ++i) {
    printf("%s\n", array[i]);
  }

  return 0;
}
