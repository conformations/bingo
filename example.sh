#! /bin/sh
make clean; make
./bingo --cell_size 140 --words_list words.txt --gv_output bingo.gv
dot -Tpng bingo.gv > bingo.png
open bingo.png

