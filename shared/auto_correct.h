/*
 * Steps:
 * - Every english word
 * - Search list and replace if close to 80% match - or num set in Kconfig
 * - Replace if last charecter is space or puctuation
 * - For faster lookup in large dict create map with addesses of each first char
 * a,b,c...
 * - create trie and store on flash to avoid creation on startup - during
 * compile time
 * - dict will not include names
 * - to cancel a correction remove the space or puctutation and it will
 * automatically replace the previous word
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct CorrectionBuffer {
  char *last_word;           // used if user reverts correction
  char **frequent_words[25]; // 25 words will be cached for fast search of
                             // mispelled words
  // TODO: maybe have struct which ranks each word in trie tree and rank by
  // usage ^^^^^
};

// inits buffer to be used in callbacks
struct CorrectionBuffer buffer = {
    .last_word = "",
    .frequent_words = {},
};
