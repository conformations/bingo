#! /bin/sh

# Number of boards to generate
num_boards=10
# Number of squares in each dimension (horizontal, vertical)
board_size=5
# Number of pixels per square in the rendered image
cell_size=140

for ((i=1; i <= num_boards ; i++))
do
  board_file=board_$i.gv
  image_file=board_$i.png
  ./bingo --board_size $board_size \
          --cell_size $cell_size \
          --words_list words.txt \
          --gv_output $board_file \
          --seed $RANDOM
  dot -Tpng $board_file > $image_file
  rm -f $board_file
done
