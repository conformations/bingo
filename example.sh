#! /bin/sh
make clean; make
./bingo --words_list words.txt --gv_template bingo_5x5.tpl --gv_output bingo.gv
dot -Tpng bingo.gv > bingo.png
open bingo.png

