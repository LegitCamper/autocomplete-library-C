import sys
import os
import string

sys.setrecursionlimit(100)

N = 26  # 26 lowercase english letters
TrieNodeInit = f"""// This file is generated from trie_dict_gen.py to create 
// the trie during compile time and save space
// https://github.com/LegitCamper/autocomplete-library-C

#define N {N} // 26 lowercase english letters

struct TrieNode {{
    struct TrieNode *children[N];
    int is_leaf;
}};    

#define S(NAME, ...) static struct TrieNode NAME = __VA_ARGS__

// trie_node structs -- aliased for smaller file size
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


def format_array(children, leaf):
    if len(children) != 0:
        children_array = []
        for k, v in children.items():
            children_array.append(f"[{string.ascii_lowercase.index(k)}] = {'&' + v}")
        children_array = f"{{{', '.join(children_array)}}}".replace("'", "")
    else:
        children_array = "{}"

    return children_array


def format_struct(word, name, children, leaf):
    children_array = format_array(children, leaf)
    line = f"S({name}, {{ .children = {children_array}, .is_leaf = {leaf} }}); \n"
    return line


def add_char_to_trie(fword, word, loc):
    if len(word) == 1:
        add_to_lines(fword, f"_{fword[:loc]}", word[0], {}, 1)
        return (word[0], f"_{fword[:loc]}")
    else:
        children = {}
        char = word[0]
        word = word[1:]

        (uchar, ufunc_name) = add_char_to_trie(fword, word, loc + 1)
        children.update({uchar: ufunc_name})

        func_name = f"_{fword[:loc]}"
        add_to_lines(fword, func_name, char, children, 0)

        return (char, func_name)


def main():
    with open("../dict.txt", "r") as file:
        dict_words = [line.strip() for line in file]

    for word in dict_words:
        add_char_to_trie(word, word, loc=1)

    header_file = "word_trie.h"
    if os.path.exists(header_file):
        os.remove(header_file)
    with open(header_file, "w") as file:
        file.write(TrieNodeInit)

        root_children = {}
        lines_to_write.sort(key=lambda x: len(x[1]), reverse=True)
        for word, name, data, children, leaf in lines_to_write:
            if len(name) == 2:
                root_children.update({data: name})
            file.write(format_struct(word, name, children, leaf))

        # make root
        file.write(
            f"struct TrieNode TRIE_ROOT = {{ .children = {format_array(root_children, 0)}, .is_leaf = 0 }};"
        )


main()
