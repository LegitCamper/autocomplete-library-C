if [ $1 == large ]; then
  cd large-fast && python ./trie_dict_gen.py
elif [$1 == small ]; then
  cd small-slow && python ./trie_dict_gen.py
else
  echo "incorrect arg: 'large' or 'small'"
fi
