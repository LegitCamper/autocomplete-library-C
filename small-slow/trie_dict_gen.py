import sys
import os
import string

sys.setrecursionlimit(100)

TrieNodeInit = """#include <stdbool.h>
#include <stdio.h>
// This file is generated from trie_dict_gen.py to create 
// the trie during compile time and save space
// https://github.com/LegitCamper/autocomplete-library-C

struct TrieNode {{
    char symbol;
    struct TrieNode *next;
    union {{
        void *leaf
        struct TrieNode *child;
    }}
}};

#define S(NAME, ...) static struct TrieNode NAME = __VA_ARGS__

// trie_node structs -- aliased for smaller file size
// varibles use upper-case names to skirt reserved names like struct, short, etc
"""

lines_to_write = []


def add_to_lines(new_word, new_name, new_data, new_children, new_leaf):
    for word, name, data, children, leaf in lines_to_write:
        if name == new_name:
            # already in list so merge children
            children.update(new_children)
            return
    # not already in list
    lines_to_write.append([new_word, new_name, new_data, new_children, new_leaf])


def format_struct(word, name, data, children, leaf):
    lines = []
    if leaf == 1:
        leaf = True
    else:
        leaf = False
    lines.append(
        f"S({name}, {{ .symbol= '{data}', .child = {{fillme}}, .next = {{fillme}} }}); \n".replace(
            "False", "false"
        ).replace("True", "true")
    )
    return lines


def uppercase_var_name(name):
    # protocol is a reserved keyword
    if name == "protocol":
        name = "protocol_"
    # upper-case reserved names like long, short, struct
    return name[0].upper() + name[1:]


def add_char_to_trie(fword, word, loc):
    if len(word) == 0:
        func_name = f"{fword[:loc]}_leaf"
        func_name = uppercase_var_name(func_name)
        previous_char = fword[len(fword) - 1]
        add_to_lines(fword, func_name, previous_char, {}, 1)
        return (previous_char, func_name)
    else:
        children = {}
        char = word[0]
        word = word[1:]

        (uchar, ufunc_name) = add_char_to_trie(fword, word, loc + 1)
        children.update({uchar: ufunc_name})

        func_name = f"{fword[:loc]}"
        func_name = uppercase_var_name(func_name)
        add_to_lines(fword, func_name, char, children, 0)

        return (char, func_name)


def main():
    with open("../shared/dict.txt", "r") as file:
        dict_words = [line.strip() for line in file]

    for word in dict_words:
        add_char_to_trie(word, word, loc=1)

    header_file = "../shared/trie_dict.h"
    if os.path.exists(header_file):
        os.remove(header_file)
    with open(header_file, "w") as file:
        file.write(TrieNodeInit)

        root_children = {}
        lines_to_write.sort(key=lambda x: len(x[1]), reverse=True)
        for word, name, data, children, leaf in lines_to_write:
            if len(name) == 1:
                root_children.update({data: name})
            file.write(format_struct(word, name, data, children, leaf))

        # make root
        file.write(
            "struct TrieNode TRIE_ROOT = {{ .children = {{fillme}}, .is_leaf = false }};"  # format_array(root_children, 0)
        )

        # concat headers
        with open("../small-slow/func.h") as infile:
            file.write("\n")
            file.write(infile.read())
        with open("../shared/auto_correct.h") as infile:
            file.write("\n\n")
            file.write(infile.read())


main()
